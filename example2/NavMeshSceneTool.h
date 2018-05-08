#ifndef __MYNAVMESHSCENE_H__
#define __MYNAVMESHSCENE_H__

#include <scene.h>
#include <agent.h>
#include <stdio.h>
#include <memory>
#include "Sample.h"

class Player : public NavMeshScene::Agent
{
public:

    void OnHitAgent(Agent* agent) override
    {
        if (!mRobot)
        {
            return;
        }
        changeDir();
    }

    void Update(float delta) override
    {
        NavMeshScene::Agent::Update(delta);

        if (!mRobot)
        {
            return;
        }

        auto v = GetVelocity();
        if (v[0] = 0 && v[1] == 0 && v[2] == 0)
        {
            changeDir();
        }
    }

public:

    void changeDir()
    {
        float vx = float((rand() % 6 + 3) * (rand() % 2 == 0 ? 1 : -1));
        float vy = float((rand() % 6 + 3) * (rand() % 2 == 0 ? 1 : -1));
        float v[3] = { vx,0, vy };
        SetVelocity(v);
    }

    bool mRobot = true;
};

class NavMeshSceneTool : public SampleTool
{
    Sample* m_sample;

public:
    NavMeshSceneTool();

    virtual int type() { return TOOL_MY_NAV_MESH_SCENE; }
    virtual void init(Sample* sample);
    virtual void reset();
    virtual void handleMenu();
    virtual void handleClick(const float* s, const float* p, bool shift);
    virtual void handleToggle();
    virtual void handleStep();
    virtual void handleUpdate(const float dt);
    virtual void handleRender();
    virtual void handleRenderOverlay(double* proj, double* model, int* view);

private:
    void drawAgent(const float* pos, float r, float h, float c, const unsigned int col);
    void doInit();

    std::shared_ptr<NavMeshScene::Scene> mScene;
    std::vector<std::shared_ptr<Player>> mAgents;
    int mMeshMode;
    int mCurrentAgent;
};

#endif
