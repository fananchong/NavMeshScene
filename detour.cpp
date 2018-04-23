#include "detour.h"
#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>
#include <DetourCommon.h>
#include <cstdio>
#include <string>
#include <cassert>

namespace NavMeshScene {

    std::unordered_map<std::string, dtNavMesh*> Detour::mStaticMesh;

#pragma pack(1)

    struct NavMeshSetHeader
    {
        int32_t magic;
        int32_t version;
        int32_t numTiles;
        dtNavMeshParams params;
    };

    struct NavMeshTileHeader
    {
        dtTileRef tileRef;
        int32_t dataSize;
    };

#pragma pack()
    static const int32_t NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T';
    static const int32_t NAVMESHSET_VERSION = 1;


    class FileReader {
    public:
        FileReader(const char* path)
            :fp(nullptr)
        {
#pragma warning(push)
#pragma warning(disable: 4996)
            fp = fopen(path, "rb");
#pragma warning(pop)
        }
        ~FileReader() {
            if (fp) {
                fclose(fp);
                fp = nullptr;
            }
        }
        operator FILE*() {
            return fp;
        }

    private:
        FILE* fp;
    };

    Detour::Detour(bool bStaticMesh, uint16_t maxNode)
        : mbStaticMesh(bStaticMesh)
        , mMaxNode(maxNode)
        , mMesh(nullptr)
        , mQuery(nullptr)
    {
        assert(maxNode != 0);
    }

    Detour::~Detour() {
        if (mQuery) {
            dtFreeNavMeshQuery(mQuery);
            mQuery = nullptr;
        }
        if (mMesh) {
            dtFreeNavMesh(mMesh);
            mMesh = nullptr;
        }
    }

    int Detour::Load(const char*path) {
        int errCode = 0;
        dtNavMesh* mesh = nullptr;
        if (mbStaticMesh)
        {
            auto it = Detour::mStaticMesh.find(path);
            if (it != Detour::mStaticMesh.end()) {
                mesh = it->second;
            }
            else {
                mesh = loadDetail(path, errCode);
                if (!errCode)
                {
                    Detour::mStaticMesh[path] = mesh;
                }
            }
        }
        else {
            mMesh = mesh = loadDetail(path, errCode);
        }

        if (errCode)
        {
            return errCode;
        }

        mQuery = dtAllocNavMeshQuery();
        if (!mQuery) {
            return 9;
        }

        dtStatus status = mQuery->init(mesh, mMaxNode);
        if (dtStatusFailed(status)) {
            return 10;
        }
        return 0;
    }

    dtNavMesh* Detour::loadDetail(const char*path, int& errCode) {
        errCode = 0;
        FileReader fp(path);
        if (fp == 0) {
            errCode = 1;
            return nullptr;
        }

        // Read header.
        NavMeshSetHeader header;
        size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
        if (readLen != 1)
        {
            errCode = 2;
            return nullptr;
        }
        if (header.magic != NAVMESHSET_MAGIC)
        {
            errCode = 3;
            return nullptr;
        }
        if (header.version != NAVMESHSET_VERSION)
        {
            errCode = 4;
            return nullptr;
        }

        dtNavMesh* mesh = dtAllocNavMesh();
        if (!mesh)
        {
            errCode = 5;
            return nullptr;
        }
        dtStatus status = mesh->init(&header.params);
        if (dtStatusFailed(status))
        {
            errCode = 6;
            return nullptr;
        }

        // Read tiles.
        for (int i = 0; i < header.numTiles; ++i)
        {
            NavMeshTileHeader tileHeader;
            readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
            if (readLen != 1)
            {
                errCode = 7;
                return nullptr;
            }

            if (!tileHeader.tileRef || !tileHeader.dataSize)
                break;

            unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
            if (!data) break;
            memset(data, 0, tileHeader.dataSize);
            readLen = fread(data, tileHeader.dataSize, 1, fp);
            if (readLen != 1)
            {
                dtFree(data);
                errCode = 8;
                return nullptr;
            }

            mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
        }
        return mesh;
    }

    bool Detour::TryMove(
        uint64_t startPolyRef,
        float startPos[3],
        float endPos[3],
        float halfExtents[3],
        const dtQueryFilter& filter,
        uint64_t& realEndPolyRef,
        float realEndPos[3],
        bool& bHit)
    {
        bHit = false;
        if (!mQuery) {
            return false;
        }
        dtPolyRef visited[16];
        int nvisited = 0;
        dtStatus status = mQuery->moveAlongSurface(
            (dtPolyRef)startPolyRef,
            startPos,
            endPos,
            &filter,
            realEndPos,
            visited,
            &nvisited,
            sizeof(visited) / sizeof(visited[0])
        );
        if (dtStatusFailed(status)) {
            return false;
        }

        realEndPolyRef = visited[nvisited - 1];
        float h = 0;
        mQuery->getPolyHeight((dtPolyRef)realEndPolyRef, realEndPos, &h);
        realEndPos[1] = h;

        if (startPolyRef == realEndPolyRef && dtVequal(realEndPos, endPos)) {
            bHit = true;
        }
        return true;
    }

    bool Detour::GetPoly(
        float pos[3],
        float halfExtents[3],
        const dtQueryFilter& filter,
        uint64_t& nearestRef,
        float nearestPt[3])
    {
        if (!mQuery) {
            return false;
        }
        dtPolyRef nRef;
        dtStatus status = mQuery->findNearestPoly(pos, halfExtents, &filter, &nRef, nearestPt);
        if (dtStatusFailed(status)) {
            return false;
        }
        nearestRef = nRef;
        return true;
    }

    bool Detour::Raycast(
        uint64_t startPolyRef,
        float startPos[3],
        float endPos[3],
        const dtQueryFilter &filter,
        bool& bHit,
        float hitPos[3])
    {
        if (!mQuery) {
            return false;
        }
        float t = 0;
        float hitNormal[3];
        dtPolyRef polys[16];
        int npolys = 0;
        dtStatus status = mQuery->raycast((dtPolyRef)startPolyRef, startPos, endPos, &filter,
            &t, hitNormal, polys, &npolys, sizeof(polys) / sizeof(polys[0]));
        if (dtStatusFailed(status)) {
            return false;
        }
        bHit = (t <= 1);
        if (bHit)
        {
            dtVlerp(hitPos, startPos, endPos, t);
            if (npolys > 0)
            {
                float h = 0;
                mQuery->getPolyHeight(polys[npolys - 1], hitPos, &h);
                hitPos[1] = h;
            }
        }
        return true;
    }

    bool Detour::RandomPosition(const dtQueryFilter* filter,
        float(*frand)(),
        uint64_t& randomRef,
        float randomPt[3])
    {
        if (!mQuery) {
            return false;
        }
        dtPolyRef ref;
        dtStatus status = mQuery->findRandomPoint(filter, frand, &ref, randomPt);
        if (dtStatusFailed(status)) {
            return false;
        }
        randomRef = ref;
        return true;
    }

}