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

    // 2. Procesamiento de enemigos con IA mejorada
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->health <= 0) {
            it = enemies.erase(it);
            continue;
        }

        // Cálculo de distancia al jugador para la IA
        float distToPlayer = std::sqrt(std::pow((p.pos.x - it->pos.x), 2) + std::pow((p.pos.y - it->pos.y), 2));
        bool canSeePlayer = (distToPlayer < it->detectionRange);

        // --- LÓGICA POR TIPO ---

        if (it->type == WALKER) {
            it->vel.y += 1500.0f * dt;
            it->pos.y += it->vel.y * dt;

            // FSM de WALKER: PATROL vs CHASE
            if (canSeePlayer) {
                it->state = CHASE;
                it->dir = (p.pos.x > it->pos.x) ? 1 : -1;
                it->speedMult = 1.8f; // Corre cuando te ve
            } else {
                it->state = PATROL;
                it->speedMult = 1.0f;
            }

            it->pos.x += it->dir * (100.0f * it->speedMult) * dt;
            it->hitbox.x = it->pos.x;
            it->hitbox.y = it->pos.y;

            // Colisión con suelo para no atravesarlo
            for (const auto& plat : level) {
                if (PhysicsEngine::AABB(it->hitbox, plat.bounds)) {
                    if (it->pos.y + it->hitbox.h > plat.bounds.y && it->pos.y < plat.bounds.y) {
                        it->pos.y = plat.bounds.y - it->hitbox.h;
                        it->vel.y = 0;
                    }
                }
            }

            // Sensor de vacío (solo en modo patrulla para no suicidarse)
            if (it->state == PATROL) {
                bool groundAhead = false;
                float sensorX = (it->dir == 1) ? it->pos.x + it->hitbox.w : it->pos.x - 5;
                Rect sensor = { sensorX, it->pos.y + it->hitbox.h + 5, 5, 5 };
                for (const auto& plat : level) {
                    if (PhysicsEngine::AABB(sensor, plat.bounds)) { groundAhead = true; break; }
                }
                if (!groundAhead) it->dir *= -1;
            }
        }

        else if (it->type == FLYER) {
            // FSM de FLYER: Acecho senoidal vs Embestida
            if (canSeePlayer) {
                it->state = CHASE;
                // Se mueve directamente hacia el jugador en ambos ejes
                float angle = std::atan2(p.pos.y - it->pos.y, p.pos.x - it->pos.x);
                it->pos.x += std::cos(angle) * 150.0f * dt;
                it->pos.y += std::sin(angle) * 150.0f * dt;
            } else {
                it->state = PATROL;
                it->pos.x += it->dir * 120.0f * dt;
                it->pos.y += std::sin(SDL_GetTicks() * 0.005f) * 2.0f;
            }
            it->hitbox.x = it->pos.x;
            it->hitbox.y = it->pos.y;
        }

        else if (it->type == TURRET) {
            // FSM de TURRET: Solo dispara si estás en rango
            if (canSeePlayer) {
                it->timer += dt;
                if (it->timer > 1.5f) { // Dispara más rápido si te ve
                    float dx = (p.pos.x + 16) - it->pos.x;
                    float dy = (p.pos.y + 24) - it->pos.y;
                    float angle = std::atan2(dy, dx);
                    bullets.push_back({{it->pos.x + 10, it->pos.y + 10}, {std::cos(angle)*300, std::sin(angle)*300}, {0,0,12,12}, true});
                    it->timer = 0;
                }
            }
        }

        // --- SISTEMA DE COMBATE (Jugador ataca) ---
        if (p.IsAttacking()) {
            Rect attackRect = p.GetAttackRect();
            if (PhysicsEngine::AABB(attackRect, it->hitbox)) {
                it->health -= 35.0f;
                // Knockback técnico
                float pushDir = (p.pos.x < it->pos.x) ? 1.0f : -1.0f;
                it->pos.x += pushDir * 25.0f; 
                if (it->type == WALKER) it->dir = (int)-pushDir;
            }
        }

        // Daño de contacto al jugador
        if (PhysicsEngine::AABB(p.hitbox, it->hitbox)) {
            p.TakeDamage(10, it->pos.x);
        }

        ++it;
    }

    // 3. Proyectiles (Se mantiene la lógica de colisión)
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

        if (destroyed || !it->active || std::abs(it->pos.x - p.pos.x) > 1200) it = bullets.erase(it);
        else ++it;
    }
}

