#include "agent.h"
#include "filter.h"
#include "scene.h"
#include "detour.h"

namespace NavMeshScene {

    Agent::Agent(Scene& scene)
        : mScene(scene)
        , mFilter(new Filter())
        , mRadius(0)
        , mCurPolyRef(0)
    {

    }

    Agent::~Agent() {

    }

    void Agent::SetPosition(float v[3]) {
        mScene.GetDetour().GetPoly(v, mHalfExtents, mFilter->Get(), mCurPolyRef, mPosition);
    }

}