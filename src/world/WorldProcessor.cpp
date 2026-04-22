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

        // --- LÓGICA DE MOVIMIENTO ---
        if (it->type == WALKER) {
            it->pos.x += it->dir * 100.0f * dt;
            it->hitbox.x = it->pos.x; 
            it->hitbox.y = it->pos.y;

            // Sensor de suelo para no caerse de plataformas
            bool groundAhead = false;
            float sensorX = (it->dir == 1) ? it->pos.x + it->hitbox.w : it->pos.x - 5;
            Rect sensor = { sensorX, it->pos.y + it->hitbox.h + 5, 5, 5 };
            
            for (const auto& plat : level) {
                if (PhysicsEngine::AABB(sensor, plat.bounds)) { 
                    groundAhead = true; 
                    break; 
                }
            }
            if (!groundAhead) it->dir *= -1; // Cambiar dirección si no hay suelo
        }
        else if (it->type == FLYER) {
            it->pos.x += it->dir * 120.0f * dt;
            // Movimiento sinusoidal para que se vea como vuelo
            it->pos.y += sin(SDL_GetTicks() * 0.005f) * 2.0f;
            it->hitbox.x = it->pos.x; 
            it->hitbox.y = it->pos.y;
            
            // Si choca con una pared (plataforma), cambia de dirección
            for (const auto& plat : level) {
                if (PhysicsEngine::AABB(it->hitbox, plat.bounds)) {
                    it->dir *= -1;
                    break;
                }
            }
        }
        else if (it->type == TURRET) {
            it->timer += dt;
            if (it->timer > 2.0f) {
                float dx = (p.pos.x + 16) - it->pos.x;
                float dy = (p.pos.y + 24) - it->pos.y;
                float angle = atan2(dy, dx);
                // Dispara bala enemiga
                bullets.push_back({{it->pos.x + 10, it->pos.y + 10}, {cos(angle)*300, sin(angle)*300}, {0,0,12,12}, true});
                it->timer = 0;
            }
        }

        // --- COLISIÓN ENEMIGO VS JUGADOR ---
        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) {
            p.TakeDamage(10, it->pos.x);
        }
        
        ++it;
    }

    // 3. Procesamiento de proyectiles
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->pos.x += it->vel.x * dt;
        it->pos.y += it->vel.y * dt;
        it->hitbox.x = it->pos.x; 
        it->hitbox.y = it->pos.y;

        bool destroyed = false;

        // Bala vs Jugador
        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) {
            p.TakeDamage(10, it->pos.x);
            destroyed = true;
        }

        // Bala vs Plataformas
        for (const auto& plat : level) {
            if (PhysicsEngine::AABB(it->hitbox, plat.bounds)) {
                destroyed = true;
                break;
            }
        }

        // Bala vs Otros Enemigos (Solo si quieres que se dañen entre ellos o si el jugador dispara)
        // Por ahora, destruye la bala si toca a un enemigo diferente al que la disparó
        for (auto& en : enemies) {
            if (PhysicsEngine::AABB(it->hitbox, en.hitbox)) {
                // Si implementas balas del jugador, aquí restarías vida: en.health -= damage;
                destroyed = true;
                break;
            }
        }

        if (destroyed || !it->active || std::abs(it->pos.x - p.pos.x) > 1000) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

