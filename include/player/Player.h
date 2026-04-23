#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "input/InputManager.h"

class Player : public Entity {
public:
    Player();
    void HandleInput(InputManager& input);
    void Update(float dt);
    void TakeDamage(float amount, float sourceX);

    float GetHealth() const { return health; }
    bool IsDashing() const { return isDashing; }
    bool IsAttacking() const { return attackTimer > 0; }
    float GetInvulTimer() const { return invulTimer; } // Nuevo: para el feedback visual
    Rect GetAttackRect() const;

private:
    float health;
    float speed;
    float jumpForce;

    float dashCooldown;
    float dashTimer;
    bool isDashing;

    float invulTimer;

    float attackTimer;
    float attackCooldown;
    int faceDir;

    void ApplyDash(float direction);
    void ApplyAttack();
};

#endif

