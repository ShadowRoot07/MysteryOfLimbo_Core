#include "world/Platform.h"
#include "player/Player.h"
#include "world/Enemy.h"
#include "physics/Collision.h"
#include <vector>
#include <cmath>
#include <SDL.h>

void ProcessWorld(Player& p, std::vector<Platform>& level, std::vector<Enemy>& enemies, std::vector<Projectile>& bullets, float dt) {
    p.isGrounded = false;

    // 1. Colisiones Jugador vs Plataformas
    for (auto& plat : level) {
        if (PhysicsEngine::AABB(p.hitbox, plat.bounds)) {
            PhysicsEngine::ResolvePlatformCollision(p, plat.bounds);
        }
    }

    // 2. Procesamiento de enemigos
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->health <= 0) { 
            it = enemies.erase(it); 
            continue; 
        }

        // Gravedad para enemigos terrestres
        if (it->type == WALKER) {
            it->vel.y += 1500.0f * dt; // Gravedad del enemigo
            it->pos.y += it->vel.y * dt;
            it->pos.x += it->dir * 100.0f * dt;
            
            it->hitbox.x = it->pos.x; 
            it->hitbox.y = it->pos.y;

            // Colisión básica de Walker con plataformas (para que no atraviesen el suelo)
            for (const auto& plat : level) {
                if (PhysicsEngine::AABB(it->hitbox, plat.bounds)) {
                    // Resolución simple: si toca suelo, frena caída
                    if (it->pos.y + it->hitbox.h > plat.bounds.y && it->pos.y < plat.bounds.y) {
                        it->pos.y = plat.bounds.y - it->hitbox.h;
                        it->vel.y = 0;
                    }
                }
            }

            // Sensor de vacío
            bool groundAhead = false;
            float sensorX = (it->dir == 1) ? it->pos.x + it->hitbox.w : it->pos.x - 5;
            Rect sensor = { sensorX, it->pos.y + it->hitbox.h + 5, 5, 5 };
            
            for (const auto& plat : level) {
                if (PhysicsEngine::AABB(sensor, plat.bounds)) { 
                    groundAhead = true; 
                    break; 
                }
            }
            if (!groundAhead) it->dir *= -1;
        }
        else if (it->type == FLYER) {
            it->pos.x += it->dir * 120.0f * dt;
            it->pos.y += sin(SDL_GetTicks() * 0.005f) * 2.0f;
            it->hitbox.x = it->pos.x; 
            it->hitbox.y = it->pos.y;
        }
        else if (it->type == TURRET) {
            it->timer += dt;
            if (it->timer > 2.0f) {
                float dx = (p.pos.x + 16) - it->pos.x;
                float dy = (p.pos.y + 24) - it->pos.y;
                float angle = atan2(dy, dx);
                bullets.push_back({{it->pos.x + 10, it->pos.y + 10}, {cos(angle)*300, sin(angle)*300}, {0,0,12,12}, true});
                it->timer = 0;
            }
        }

        // Colisión Enemigo vs Jugador (Knockback)
        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) {
            p.TakeDamage(10, it->pos.x);
        }
        
        ++it;
    }

    // 3. Proyectiles
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->pos.x += it->vel.x * dt;
        it->pos.y += it->vel.y * dt;
        it->hitbox.x = it->pos.x; it->hitbox.y = it->pos.y;

        bool destroyed = false;
        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) {
            p.TakeDamage(10, it->pos.x);
            destroyed = true;
        }

        for (const auto& plat : level) {
            if (PhysicsEngine::AABB(it->hitbox, plat.bounds)) { destroyed = true; break; }
        }

        if (destroyed || !it->active || std::abs(it->pos.x - p.pos.x) > 1000) it = bullets.erase(it);
        else ++it;
    }
}

