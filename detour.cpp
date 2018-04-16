#include "detour.h"
#include <cstdio>
#include <string>
#include <cassert>

struct NavMeshSetHeader
{
    int magic;
    int version;
    int numTiles;
    dtNavMeshParams params;
};
struct NavMeshTileHeader
{
    dtTileRef tileRef;
    int dataSize;
};
static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T';
static const int NAVMESHSET_VERSION = 1;


class FileReader {
public:
    FileReader(const char*path)
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

Detour::Detour(uint16_t maxNode)
    : mMaxNode(maxNode)
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
    FileReader fp(path);
    if (fp == 0) {
        return 1;
    }

    // Read header.
    NavMeshSetHeader header;
    size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
    if (readLen != 1)
    {
        return 2;
    }
    if (header.magic != NAVMESHSET_MAGIC)
    {
        return 3;
    }
    if (header.version != NAVMESHSET_VERSION)
    {
        return 4;
    }

    dtNavMesh* mesh = dtAllocNavMesh();
    if (!mesh)
    {
        return 5;
    }
    dtStatus status = mesh->init(&header.params);
    if (dtStatusFailed(status))
    {
        return 6;
    }

    // Read tiles.
    for (int i = 0; i < header.numTiles; ++i)
    {
        NavMeshTileHeader tileHeader;
        readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
        if (readLen != 1)
        {
            return 7;
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
            return 8;
        }

        mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
    }

    mQuery = dtAllocNavMeshQuery();
    if (!mQuery) {
        return 9;
    }

    status = mQuery->init(mesh, mMaxNode);
    if (dtStatusFailed(status)) {
        return 10;
    }
    return 0;
}