#include "player/Player.h"
#include <cmath>

Player::Player() {
    pos = {100.0f, 100.0f};
    vel = {0.0f, 0.0f};
    hitbox = {0, 0, 32.0f, 48.0f};
    isGrounded = false;
    health = 100.0f;
    speed = 300.0f;
    jumpForce = -600.0f;

    elementSlot1 = NONE;
    elementSlot2 = NONE;
    jumpCount = 0;
    maxJumps = 1;
    isLiquid = false;
    liquidTimer = 0.0f;

    // Oscuridad inicial
    shadowMark = {0, 0};
    hasMark = false;

    dashCooldown = dashTimer = 0.0f;
    isDashing = false;
    invulTimer = attackTimer = attackCooldown = 0.0f;
    faceDir = 1;
}

void Player::SetElements(ElementType slot1, ElementType slot2) {
    elementSlot1 = slot1;
    elementSlot2 = slot2;
    maxJumps = (HasElement(AIR)) ? 2 : 1;
    speed = (HasElement(LIGHT)) ? 450.0f : 300.0f;
}

bool Player::HasElement(ElementType type) const {
    return (elementSlot1 == type || elementSlot2 == type);
}

void Player::HandleInput(InputManager& input) {
    if (isDashing) return;

    vel.x = 0;
    if (input.IsKeyDown(SDL_SCANCODE_LEFT)) { vel.x = -speed; faceDir = -1; }
    else if (input.IsKeyDown(SDL_SCANCODE_RIGHT)) { vel.x = speed; faceDir = 1; }

    if (input.IsKeyPressed(SDL_SCANCODE_Z)) {
        if (isGrounded) {
            vel.y = jumpForce;
            isGrounded = false;
            jumpCount = 1;
        } else if (jumpCount < maxJumps) {
            vel.y = jumpForce * 0.85f;
            jumpCount++;
        }
    }

    if (input.IsKeyPressed(SDL_SCANCODE_F) && dashCooldown <= 0) ApplyDash((float)faceDir);
    if (input.IsKeyPressed(SDL_SCANCODE_X) && attackCooldown <= 0) ApplyAttack();

    // Botón C para habilidad del Slot 1, Botón V para Slot 2 (Sugerencia)
    if (input.IsKeyPressed(SDL_SCANCODE_C)) ExecuteActiveSkill();
}

void Player::ExecuteActiveSkill() {
    // 1. Lógica de AGUA
    if (HasElement(WATER) && !isLiquid) {
        isLiquid = true;
        liquidTimer = 1.0f;
    }
    
    // 2. Lógica de OSCURIDAD (Marcado y TP)
    if (HasElement(DARKNESS)) {
        if (!hasMark) {
            shadowMark = pos; // Colocamos la marca
            hasMark = true;
        } else {
            pos = shadowMark; // Teletransporte
            hasMark = false;
            vel = {0, 0}; // Cancelar inercia al aparecer
        }
    }

    // 3. Lógica de TIERRA (Aquí llamaremos al sistema de plataformas)
    // Nota: Esto requiere pasar la referencia al vector de plataformas en el WorldProcessor
}

void Player::Update(float dt) {
    if (invulTimer > 0) invulTimer -= dt;
    if (dashCooldown > 0) dashCooldown -= dt;
    if (attackCooldown > 0) attackCooldown -= dt;
    if (attackTimer > 0) attackTimer -= dt;

    if (liquidTimer > 0) {
        liquidTimer -= dt;
        if (liquidTimer <= 0) isLiquid = false;
    }

    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) isDashing = false;
    } else {
        vel.y += 1800.0f * dt;
    }

    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    hitbox.x = pos.x;
    hitbox.y = pos.y;

    if (isGrounded) jumpCount = 0;
}

void Player::TakeDamage(float amount, float sourceX) {
    if (isLiquid) return; 
    if (invulTimer <= 0 && !isDashing) {
        health -= amount;
        invulTimer = 1.0f;
        float knockDir = (pos.x + hitbox.w/2 > sourceX) ? 1.0f : -1.0f;
        vel.x = knockDir * 400.0f;
        vel.y = -300.0f;
        isGrounded = false;
    }
}

void Player::ApplyDash(float dir) {
    isDashing = true;
    dashTimer = 0.15f;
    dashCooldown = 0.6f;
    vel.x = dir * speed * 3.5f;
    vel.y = 0;
}

void Player::ApplyAttack() {
    attackTimer = 0.25f;
    attackCooldown = 0.4f;
}

Rect Player::GetAttackRect() const {
    float range = 50.0f;
    float height = 40.0f;
    float attackX = (faceDir == 1) ? pos.x + hitbox.w : pos.x - range;
    return { attackX, pos.y + 4, range, height };
}

