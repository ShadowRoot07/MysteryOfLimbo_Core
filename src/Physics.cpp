// src/Physics.cpp
#include "Common.h"

bool CheckCollision(Rect a, Rect b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

void ApplyGravity(Entity& e, float dt) {
    const float GRAVITY = 900.0f;
    if (!e.isGrounded) {
        e.vel.y += GRAVITY * dt;
    }
}

