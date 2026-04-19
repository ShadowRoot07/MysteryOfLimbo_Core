#include "physics/Collision.h"
#include "world/Platform.h"
#include <cmath>

void PhysicsEngine::ResolvePlatformCollision(Entity& e, Rect p) {
    // 1. Calcular el centro de ambos rectángulos
    float playerCenterX = e.pos.x + e.hitbox.w / 2.0f;
    float playerCenterY = e.pos.y + e.hitbox.h / 2.0f;
    float platformCenterX = p.x + p.w / 2.0f;
    float platformCenterY = p.y + p.h / 2.0f;

    // 2. Calcular la distancia entre centros
    float dx = playerCenterX - platformCenterX;
    float dy = playerCenterY - platformCenterY;

    // 3. Calcular la penetración en cada eje
    float combinedHalfWidths = (e.hitbox.w + p.w) / 2.0f;
    float combinedHalfHeights = (e.hitbox.h + p.h) / 2.0f;

    float overlapX = combinedHalfWidths - std::abs(dx);
    float overlapY = combinedHalfHeights - std::abs(dy);

    // 4. Resolver en el eje de menor penetración para evitar "teletransportes" raros
    if (overlapX < overlapY) {
        if (dx > 0) e.pos.x += overlapX; // Empujar a la derecha
        else e.pos.x -= overlapX;        // Empujar a la izquierda
        e.vel.x = 0;
    } else {
        if (dy > 0) {
            e.pos.y += overlapY;         // Empujar hacia abajo (chocó techo)
            e.vel.y = 0;
        } else {
            e.pos.y -= overlapY;         // Empujar hacia arriba (pisó suelo)
            e.vel.y = 0;
            e.isGrounded = true;         // Recuperar salto
        }
    }
}

