#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>
#include <cstdint>

namespace NavMeshScene {

    class Detour {
    public:

        Detour(uint16_t maxNode = 2048);
        ~Detour();

        int Load(const char* path);


    private:
        int mMaxNode;
        dtNavMesh* mMesh;
        dtNavMeshQuery* mQuery;
    };

}