#ifndef __MYNAVMESHSCENE_H__
#define __MYNAVMESHSCENE_H__

#include <scene.h>
#include <agent.h>
#include <stdio.h>
#include <memory>
#include "Sample.h"

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
    std::shared_ptr<NavMeshScene::Agent> mAgent;
    int mMeshMode;
};

#endif
