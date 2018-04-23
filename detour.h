#ifndef __NMS_DETOUR_H__
#define __NMS_DETOUR_H__

#include <cstdint>
#include <unordered_map>

class dtNavMesh;
class dtNavMeshQuery;
class dtQueryFilter;

namespace NavMeshScene {

    class Detour {
    public:

        Detour(bool bStaticMesh = true, uint16_t maxNode = 2048);
        virtual ~Detour();

        int Load(const char* path);

        bool TryMove(
            uint64_t startPolyRef,
            float startPos[3],
            float endPos[3],
            float halfExtents[3],
            const dtQueryFilter& filter,
            uint64_t& realEndPolyRef,
            float realEndPos[3],
            bool& bHit);

        bool GetPoly(
            float pos[3],
            float halfExtents[3],
            const dtQueryFilter& filter,
            uint64_t& nearestRef,
            float nearestPt[3]);

        bool Raycast(
            uint64_t startPolyRef,
            float startPos[3],
            float endPos[3],
            const dtQueryFilter &filter,
            bool& bHit,
            float hitPos[3]);

        bool RandomPosition(
            const dtQueryFilter* filter,
            float(*frand)(),
            uint64_t& randomRef,
            float randomPt[3]);

    protected:
        dtNavMesh* loadDetail(const char*path, int& errCode);

        bool mbStaticMesh;
        int mMaxNode;
        dtNavMesh* mMesh;
        dtNavMeshQuery* mQuery;
        static std::unordered_map<std::string, dtNavMesh*> mStaticMesh;
    };

}

#endif