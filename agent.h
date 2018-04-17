#ifndef __NMS_AGENT_H__
#define __NMS_AGENT_H__

#include <memory>

namespace NavMeshScene {

    class Filter;
    class Scene;

    class Agent {
    public:

        Agent(Scene& scene);
        ~Agent();

        inline float* GetHalfExtents() { return mHalfExtents; }
        inline float* GetPosition() { return mPosition; }
        inline float* GetVelocity() { return mVelocity; }
        inline float GetRadius() { return mRadius; }
        inline Filter& GetFilter() { return *mFilter; }

        inline void SetHalfExtents(float v[3]) { mHalfExtents[0] = v[0]; mHalfExtents[1] = v[1]; mHalfExtents[2] = v[2]; }
        void SetPosition(float v[3]);
        inline void SetVelocity(float v[3]) { mVelocity[0] = v[0]; mVelocity[1] = v[1]; mVelocity[2] = v[2]; }
        inline void SetRadius(float v) { mRadius = v; }

    private:
        float mHalfExtents[3];
        float mPosition[3];
        float mVelocity[3];
        float mRadius;
        uint64_t mCurPolyRef;
        std::unique_ptr<Filter> mFilter;
        Scene& mScene;
    };

}

#endif