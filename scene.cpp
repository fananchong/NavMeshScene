#include "scene.h"
#include "agent.h"
#include "detour.h"
#include "filter.h"

namespace NavMeshScene {

    Scene::Scene()
        : mDetour(new Detour(16))
        , mDefaultFilter(new Filter())
    {

    }

    Scene::~Scene() {

    }

    int Scene::Load(const char* path) {
        return mDetour->Load(path);
    }

    void Scene::Simulation(float delta) {
        for (auto it = mAgents.begin(); it != mAgents.end(); it++) {
            auto &agent = it->second;
            agent->Update(delta);
        }
    }

    void Scene::AddAgent(uint64_t id, const std::shared_ptr<Agent>& agent) {
        if (id && agent) {
            mAgents[id] = agent;
            agent->SetScene(this);
        }
    }

    void Scene::RemoveAgent(uint64_t id) {
        auto it = mAgents.find(id);
        if (it != mAgents.end()) {
            mAgents.erase(it);
        }
    }

}