#include "NavMeshSceneTool.h"

NavMeshSceneTool::NavMeshSceneTool() : m_sample(0)
{
}

void NavMeshSceneTool::init(Sample* sample)
{
    m_sample = sample;
}

void NavMeshSceneTool::handleMenu()
{
}

void NavMeshSceneTool::handleClick(const float* /*s*/, const float* p, bool shift)
{
}

void NavMeshSceneTool::handleStep()
{
}

void NavMeshSceneTool::handleToggle()
{
}

void NavMeshSceneTool::handleUpdate(const float /*dt*/)
{

}

void NavMeshSceneTool::reset()
{
}

void NavMeshSceneTool::handleRender()
{

}

void NavMeshSceneTool::handleRenderOverlay(double* proj, double* model, int* view)
{

}

void NavMeshSceneTool::drawAgent(const float* pos, float r, float h, float c, const unsigned int col)
{
    duDebugDraw& dd = m_sample->getDebugDraw();

    dd.depthMask(false);

    // Agent dimensions.	
    duDebugDrawCylinderWire(&dd, pos[0] - r, pos[1] + 0.02f, pos[2] - r, pos[0] + r, pos[1] + h, pos[2] + r, col, 2.0f);

    duDebugDrawCircle(&dd, pos[0], pos[1] + c, pos[2], r, duRGBA(0, 0, 0, 64), 1.0f);

    unsigned int colb = duRGBA(0, 0, 0, 196);
    dd.begin(DU_DRAW_LINES);
    dd.vertex(pos[0], pos[1] - c, pos[2], colb);
    dd.vertex(pos[0], pos[1] + c, pos[2], colb);
    dd.vertex(pos[0] - r / 2, pos[1] + 0.02f, pos[2], colb);
    dd.vertex(pos[0] + r / 2, pos[1] + 0.02f, pos[2], colb);
    dd.vertex(pos[0], pos[1] + 0.02f, pos[2] - r / 2, colb);
    dd.vertex(pos[0], pos[1] + 0.02f, pos[2] + r / 2, colb);
    dd.end();

    dd.depthMask(true);
}
