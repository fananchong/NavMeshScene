#include "agent.h"
#include "filter.h"
#include "scene.h"
#include "detour.h"
#include <DetourNavMeshQuery.h>
#include <DetourCommon.h>

namespace NavMeshScene {

    const float ZERO[3] = { 0,0,0 };

    Agent::Agent()
        : mScene(nullptr)
        , mFilter(nullptr)
        , mCurPolyRef(0)
    {
        dtVcopy(mHalfExtents, DEFAULT_HALF_EXTENTS);
        dtVcopy(mPosition, ZERO);
        dtVcopy(mVelocity, ZERO);
    }

    Agent::~Agent() {

    }

    void Agent::Update(float delta) {
        if (mVelocity[0] == 0 && mVelocity[1] == 0 && mVelocity[2] == 0) {
            return;
        }
        float endPos[3] = {
            mPosition[0] + mVelocity[0] * delta,
            mPosition[1] + mVelocity[1] * delta,
            mPosition[2] + mVelocity[2] * delta
        };
        uint64_t realEndPolyRef;
        float realEndPos[3];
        bool bHit;
        if (!TryMove(endPos, realEndPolyRef, realEndPos, bHit)) {
            return;
        }
        mCurPolyRef = realEndPolyRef;
        dtVcopy(mPosition, realEndPos);
        if (bHit)
        {
            dtVcopy(mVelocity, ZERO);
        }
    }

    bool Agent::TryMove(float endPos[3], uint64_t& realEndPolyRef, float realEndPos[3], bool& bHit) {
        if (mScene)
        {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            return mScene->GetDetour().TryMove(mCurPolyRef, mPosition, endPos, mHalfExtents, filter.Get(), realEndPolyRef, realEndPos, bHit);
        }
        return false;
    }

    void Agent::SetPosition(float v[3]) {
        if (mScene) {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            mScene->GetDetour().GetPoly(v, mHalfExtents, filter.Get(), mCurPolyRef, mPosition);
        }
        else {
            dtVcopy(mPosition, v);
        }
    }

}