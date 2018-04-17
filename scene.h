#ifndef __NMS_SCENE_H__
#define __NMS_SCENE_H__

#include <memory>
#include <unordered_map>

namespace NavMeshScene {

    class Agent;
    class Detour;
    class Filter;

    class Scene {
    public:

        Scene();
        virtual ~Scene();

        int Load(const char* path);
        void Simulation(float delta);
        void AddAgent(uint64_t id, const std::shared_ptr<Agent>& agent);
        void RemoveAgent(uint64_t id);

    public:
        inline Detour& GetDetour() { return *mDetour; }
        inline Filter& GetDefaultFilter() { return *mDefaultFilter; }

    protected:
        std::unique_ptr<Detour> mDetour;
        std::unordered_map<uint64_t, std::shared_ptr<Agent>> mAgents;
        std::unique_ptr<Filter> mDefaultFilter;
    };

}

#endif