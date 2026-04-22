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
    dashCooldown = dashTimer = 0.0f;
    isDashing = false;
    invulTimer = attackTimer = attackCooldown = 0.0f;
    faceDir = 1;
}

void Player::HandleInput(InputManager& input) {
    // Si estamos en invulnerabilidad reciente por golpe, el control es limitado (opcional)
    // Pero si estamos dasheando, el control se bloquea totalmente
    if (isDashing) return;

    vel.x = 0;
    
    // Joystick o Teclado (Ahora soporta eje Y aunque no se use para caminar)
    if (input.IsKeyDown(SDL_SCANCODE_LEFT)) { vel.x = -speed; faceDir = -1; }
    else if (input.IsKeyDown(SDL_SCANCODE_RIGHT)) { vel.x = speed; faceDir = 1; }

    if (input.IsKeyPressed(SDL_SCANCODE_Z)) {
        if (isGrounded) {
            vel.y = jumpForce;
            isGrounded = false;
        }
    }

    if (input.IsKeyPressed(SDL_SCANCODE_F) && dashCooldown <= 0) ApplyDash((float)faceDir);
    if (input.IsKeyPressed(SDL_SCANCODE_X) && attackCooldown <= 0) ApplyAttack();
}

void Player::ApplyDash(float dir) {
    isDashing = true;
    dashTimer = 0.15f;
    dashCooldown = 0.6f;
    vel.x = dir * speed * 3.5f;
    vel.y = 0;
}

void Player::ApplyAttack() {
    attackTimer = 0.2f;
    attackCooldown = 0.4f;
}

Rect Player::GetAttackRect() const {
    float range = 40.0f;
    return { (faceDir == 1) ? pos.x + hitbox.w : pos.x - range, pos.y + 10, range, 30 };
}

void Player::Update(float dt) {
    if (invulTimer > 0) invulTimer -= dt;
    if (dashCooldown > 0) dashCooldown -= dt;
    if (attackCooldown > 0) attackCooldown -= dt;
    if (attackTimer > 0) attackTimer -= dt;

    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) isDashing = false;
    } else {
        // Gravedad normal solo si no dasheamos
        vel.y += 1800.0f * dt;
    }

    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    hitbox.x = pos.x; hitbox.y = pos.y;
}

void Player::TakeDamage(float amount, float sourceX) {
    // El Dash da "frames de alma" (invulnerabilidad) según el diseño
    if (invulTimer <= 0 && !isDashing) {
        health -= amount;
        invulTimer = 0.8f; // Tiempo de parpadeo

        // Lógica de Knockback: Empuje contrario al origen
        float knockDir = (pos.x + hitbox.w/2 > sourceX) ? 1.0f : -1.0f;
        vel.x = knockDir * 400.0f;
        vel.y = -300.0f; // Pequeño salto por el impacto
        isGrounded = false;
    }
}

