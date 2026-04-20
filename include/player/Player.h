#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include "input/InputManager.h"

class Player : public Entity {
public:
    Player();
    void HandleInput(InputManager& input);
    void Update(float dt);
    void TakeDamage(float amount);

    float GetHealth() const { return health; }
    bool IsDashing() const { return isDashing; }
    bool IsAttacking() const { return attackTimer > 0; }
    Rect GetAttackRect() const;

private:
    float health; // Eliminamos maxHealth para limpiar el warning
    float speed;
    float jumpForce;
    
    // Lógica de Dash
    float dashCooldown;
    float dashTimer;
    bool isDashing;
    
    // Control de daño e invulnerabilidad
    float invulTimer;
    
    // Combate
    float attackTimer;
    float attackCooldown;
    int faceDir; // 1 derecha, -1 izquierda

    void ApplyDash(float direction);
    void ApplyAttack();
};

#endif

