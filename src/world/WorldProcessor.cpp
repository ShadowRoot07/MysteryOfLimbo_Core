#include "world/Platform.h"
#include "player/Player.h"
#include "world/Enemy.h"
#include "physics/Collision.h"
#include <vector> // <--- Esencial para que funcione std::vector

void ProcessWorld(Player& p, std::vector<Platform>& level, std::vector<Enemy>& enemies, std::vector<Projectile>& bullets, float dt) {
    p.isGrounded = false;
    
    // Colisiones con plataformas
    for (auto& plat : level) {
        if (PhysicsEngine::AABB(p.hitbox, plat.bounds)) {
            PhysicsEngine::ResolvePlatformCollision(p, plat.bounds);
            if (plat.type == SPIKE) p.TakeDamage(25);
        }
    }

    // Lógica de Ataque del Jugador
    if (p.IsAttacking()) {
        Rect atk = p.GetAttackRect();
        for (auto& e : enemies) {
            if (PhysicsEngine::AABB(atk, e.hitbox)) {
                e.health -= 25;
                e.pos.x += (p.pos.x < e.pos.x ? 20 : -20); // Knockback
            }
        }
    }

    // Lógica de Enemigos e IA
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->health <= 0) {
            it = enemies.erase(it);
            continue;
        }

        if (it->type == WALKER) {
            it->pos.x += it->dir * 100.0f * dt;
            it->timer += dt;
            if (it->timer > 2.0f) { it->dir *= -1; it->timer = 0; }
        }
        else if (it->type == FLYER) {
            it->pos.x += it->dir * 150.0f * dt;
            it->pos.y += (float)sin(SDL_GetTicks() * 0.005f) * 2.0f;
        }
        else if (it->type == TURRET) {
            it->timer += dt;
            if (it->timer > 1.5f) {
                bullets.push_back({{it->pos.x, it->pos.y}, {(p.pos.x < it->pos.x ? -300.0f : 300.0f), 0}, {0,0,10,10}, true});
                it->timer = 0;
            }
        }

        it->hitbox.x = it->pos.x;
        it->hitbox.y = it->pos.y;
        
        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) p.TakeDamage(20);
        ++it;
    }

    // Procesar Proyectiles
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        if (!it->active) {
            it = bullets.erase(it);
            continue;
        }
        it->pos.x += it->vel.x * dt;
        it->hitbox.x = it->pos.x;
        it->hitbox.y = it->pos.y;

        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) {
            p.TakeDamage(10);
            it->active = false;
        }
        
        // Auto-destruir balas fuera de rango (opcional)
        if (std::abs(it->pos.x - p.pos.x) > 1000) it->active = false;
        
        ++it;
    }
}

