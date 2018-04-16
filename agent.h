#include "vector3.h"

namespace NavMeshScene {

    class Agent {
    public:

        Agent();
        ~Agent();

        inline Vector3 GetPosition() { return mPosition; }
        inline Vector3 GetVelocity() { return mVelocity; }
        inline float GetRadius() { return mRadius; }

        inline void SetPosition(const Vector3& v) { mPosition = v; }
        inline void SetVelocity(const Vector3& v) { mVelocity = v; }
        inline void SetRadius(float v) { mRadius = v; }

    private:
        Vector3 mPosition;
        Vector3 mVelocity;
        float mRadius;
    };

}