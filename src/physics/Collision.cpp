#include "physics/Collision.h"

bool PhysicsEngine::AABB(Rect a, Rect b) {
    // Si alguna de estas condiciones es verdadera, NO hay colisión
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

