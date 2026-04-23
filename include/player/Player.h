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

    void SetElements(ElementType slot1, ElementType slot2);
    bool HasElement(ElementType type) const;

    float GetHealth() const { return health; }
    bool IsDashing() const { return isDashing; }
    bool IsAttacking() const { return attackTimer > 0; }
    float GetInvulTimer() const { return invulTimer; }
    Rect GetAttackRect() const;
    
    // Para que Tierra pueda posicionar la plataforma
    Vector2 GetPos() const { return pos; }

private:
    float health;
    float speed;
    float jumpForce;

    ElementType elementSlot1;
    ElementType elementSlot2;

    // Lógica de Aire
    int jumpCount;
    int maxJumps;

    // Lógica de Agua
    bool isLiquid;
    float liquidTimer;

    // Lógica de Oscuridad (NUEVO)
    Vector2 shadowMark;
    bool hasMark;

    float dashCooldown;
    float dashTimer;
    bool isDashing;

    float invulTimer;
    float attackTimer;
    float attackCooldown;
    int faceDir;

    void ApplyDash(float direction);
    void ApplyAttack();
    void ExecuteActiveSkill();
};

#endif

