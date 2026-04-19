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
    
    // Getters para el estado
    float GetHealth() const { return health; }
    bool IsInvulnerable() const { return invulTimer > 0; }

private:
    float health;
    float maxHealth;
    float speed;
    float jumpForce;
    
    // Lógica de Dash
    float dashCooldown;
    float dashTimer;
    bool isDashing;
    
    // Control de daño
    float invulTimer;

    void ApplyDash(float direction);
};

#endif

