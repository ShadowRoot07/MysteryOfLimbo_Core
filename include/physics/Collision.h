#ifndef COLLISION_H
#define COLLISION_H

#include "Common.h"

class PhysicsEngine {
public:
    static bool AABB(Rect a, Rect b);
    static void ResolvePlatformCollision(Entity& e, Rect platform);
};

#endif

