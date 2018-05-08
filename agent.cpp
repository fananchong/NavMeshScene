#include "agent.h"
#include "filter.h"
#include "scene.h"
#include "detour.h"
#include <DetourNavMeshQuery.h>
#include <DetourCommon.h>

namespace NavMeshScene {

    const float ZERO[3] = { 0,0,0 };

    Agent::Agent()
        : mId(0)
        , mScene(nullptr)
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
        if (auto agent = checkPosByAOI(mPosition[0], mPosition[2], endPos[0], endPos[2], true))
        {
            // ֹͣ
            dtVcopy(mVelocity, ZERO);
            OnHitAgent(agent);
            return;
        }
        uint64_t realEndPolyRef;
        float realEndPos[3];
        if (!TryMove(endPos, realEndPolyRef, realEndPos)) {
            return;
        }
        mCurPolyRef = realEndPolyRef;
        dtVcopy(mPosition, realEndPos);
        if (fabs(X - mPosition[0]) >= FLT_EPSILON || fabs(Y - mPosition[2]) >= FLT_EPSILON)
        {
            X = mPosition[0];
            Y = mPosition[2];
            mScene->Update(this);
        }
    }

    bool Agent::TryMove(float endPos[3], uint64_t& realEndPolyRef, float realEndPos[3]) {
        if (mScene)
        {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            return mScene->GetDetour().TryMove(
                mCurPolyRef,
                mPosition,
                endPos,
                mHalfExtents,
                filter.Get(),
                realEndPolyRef,
                realEndPos);
        }
        return false;
    }

    void Agent::SetPosition(float v[3]) {
        if (mScene) {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            mScene->GetDetour().GetPoly(v, mHalfExtents, filter.Get(), mCurPolyRef, mPosition);
            X = mPosition[0];
            Y = mPosition[2];
            mScene->Update(this);
        }
    }

    float randf()
    {
        return (float)(rand() / (float)RAND_MAX);
    }

    void Agent::RandomPosition() {
        if (mScene) {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
        LABLE_RANDOM:
            mScene->GetDetour().RandomPosition(mHalfExtents, &filter.Get(), randf, mCurPolyRef, mPosition);
            if (checkPosByAOI(mPosition[0], mPosition[2], mPosition[0], mPosition[2], false))
            {
                goto LABLE_RANDOM;
            }
            X = mPosition[0];
            Y = mPosition[2];
            mScene->Update(this);
        }
    }

    bool Agent::Raycast(float endPos[3], bool& bHit, float hitPos[3]) {
        if (mScene) {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            return mScene->GetDetour().Raycast(
                mCurPolyRef,
                mPosition,
                endPos,
                filter.Get(),
                bHit,
                hitPos);
        }
        return false;
    }

    Agent* Agent::checkPosByAOI(float srcX, float srcY, float& dstX, float& dstY, bool bMove)
    {
        float pos1[3] = { srcX, 0, srcY };
        float pos2[3] = { dstX, 0, dstY };
        if (dtVequal(pos1, pos2) && !bMove)
        {
            return nullptr;
        }
        aoi::Rect rect(dstX - mHalfExtents[0], dstX + mHalfExtents[0], dstY - mHalfExtents[2], dstY + mHalfExtents[2]);
        auto agents = mScene->Query(rect);
        for (Agent* a = (Agent*)agents; a; a = (Agent*)a->Next())
        {
            if (a == this)
            {
                continue;
            }
            aoi::Rect tempRect(a->getRect());
            if (rect.Intersects(tempRect))
            {
                if (bMove)
                {
                    if (!tempRect.Contains(srcX, srcY))
                    {
                        return a;
                    }
                    return nullptr;
                }
                else
                {
                    return a;
                }
            }
        }
        return nullptr;
    }
}