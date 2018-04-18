#ifndef __NMS_AGENT_H__
#define __NMS_AGENT_H__

#include <memory>

namespace NavMeshScene {

    const float DEFAULT_HALF_EXTENTS[3] = { 2, 4, 2 };

    class Filter;
    class Scene;

    class Agent {
    public:

        Agent();
        virtual ~Agent();

        inline float* GetPosition() { return mPosition; }
        inline float* GetHalfExtents() { return mHalfExtents; }
        inline float* GetVelocity() { return mVelocity; }
        inline Filter& GetFilter() { return *mFilter; }

        inline void SetHalfExtents(float v[3]) { mHalfExtents[0] = v[0]; mHalfExtents[1] = v[1]; mHalfExtents[2] = v[2]; }
        inline void SetVelocity(float v[3]) { mVelocity[0] = v[0]; mVelocity[1] = v[1]; mVelocity[2] = v[2]; }
        inline void SetFilter(const std::shared_ptr<Filter>& filter) { mFilter = filter; }
        inline void SetScene(Scene* scene) { mScene = scene; }

        void Update(float delta);
        void SetPosition(float v[3]);
        void RandomPosition();
        bool Raycast(float endPos[3], bool& bHit, float hitPos[3]);

    protected:
        bool TryMove(float endPos[3], uint64_t& realEndPolyRef, float realEndPos[3], bool& bHit);

        float mHalfExtents[3];
        float mPosition[3];
        float mVelocity[3];
        uint64_t mCurPolyRef;
        std::shared_ptr<Filter> mFilter;
        Scene* mScene;
    };

}

#endif