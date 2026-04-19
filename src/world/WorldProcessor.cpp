#include "world/Platform.h"
#include "player/Player.h"
#include "physics/Collision.h"
#include <vector>

void ProcessWorldInteractions(Player& p, const std::vector<Platform>& level) {
    // Por defecto, si no toca nada, está en el aire
    p.isGrounded = false;

    for (const auto& plat : level) {
        // Usamos la función que acabamos de crear en Collision.cpp
        if (PhysicsEngine::AABB(p.hitbox, plat.bounds)) {
            
            // Si es plataforma sólida o pinchos, resolvemos la posición
            PhysicsEngine::ResolvePlatformCollision(p, plat.bounds);

            // Lógica extra para peligros
            if (plat.type == SPIKE) {
                p.TakeDamage(25.0f); // El daño que definimos
                // Un pequeño salto hacia atrás para que no se muera instantáneamente
                p.vel.y = -300.0f;
            }
        }
    }
}

