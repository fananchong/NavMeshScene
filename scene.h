#include <memory>
#include <unordered_map>

namespace NavMeshScene {

    class Agent;
    class Detour;

    class Scene {
    public:

        Scene();
        ~Scene();

        int Load(const char* path);
        void AddAgent(const std::shared_ptr<Agent>& agent);
        void Simulation(float delta);

    private:
        std::unique_ptr<Detour> mDetour;
        std::unordered_map<uint64_t, std::shared_ptr<Agent>> mAgents;
    };

}