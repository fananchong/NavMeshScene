#include "scene.h"
#include "agent.h"
#include "detour.h"

namespace NavMeshScene {

    Scene::Scene()
        : mDetour(new Detour(1))
    {

    }

    Scene::~Scene() {

    }

    int Scene::Load(const char* path) {
        return mDetour->Load(path);
    }

    void Scene::AddAgent(uint64_t id, const std::shared_ptr<Agent>& agent) {
        mAgents[id] = agent;
    }

    void Scene::Simulation(float delta) {

    }

}