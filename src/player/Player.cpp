#include "player/Player.h"
#include <iostream>
#include <algorithm>

Player::Player() {
    pos = {100.0f, 100.0f};
    vel = {0.0f, 0.0f};
    hitbox = {100.0f, 100.0f, 32.0f, 48.0f}; 
    isGrounded = false;
    health = 100.0f;
    maxHealth = 100.0f;
    speed = 300.0f;
    jumpForce = -600.0f; 
    dashCooldown = 0.0f;
    dashTimer = 0.0f;
    isDashing = false;
    invulTimer = 0.0f;
}

void Player::HandleInput(InputManager& input) {
    // --- DEBUG TACTIL (TELETRANSPORTE) ---
    if (input.WasMouseClicked()) {
        pos.x = (float)input.GetMouseX() - (hitbox.w / 2.0f);
        pos.y = (float)input.GetMouseY() - (hitbox.h / 2.0f);
        vel.y = 0; // Resetear caída al soltarlo
        input.ResetMouseClick();
        return; // Prioridad al toque
    }

    if (isDashing) return;

    vel.x = 0;
    if (input.IsKeyDown(SDL_SCANCODE_LEFT)) vel.x = -speed;
    if (input.IsKeyDown(SDL_SCANCODE_RIGHT)) vel.x = speed;

    if (input.IsKeyPressed(SDL_SCANCODE_Z) && isGrounded) {
        vel.y = jumpForce;
        isGrounded = false;
    }

    if (input.IsKeyPressed(SDL_SCANCODE_X) && dashCooldown <= 0) {
        float dir = (vel.x >= 0) ? 1.0f : -1.0f;
        ApplyDash(dir);
    }
}

void Player::ApplyDash(float direction) {
    isDashing = true;
    dashTimer = 0.2f;
    dashCooldown = 0.8f;
    vel.x = direction * speed * 3.0f;
    vel.y = 0; 
}

void Player::Update(float dt) {
    if (invulTimer > 0) invulTimer -= dt;
    if (dashCooldown > 0) dashCooldown -= dt;

    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) isDashing = false;
    } else {
        // --- LA PIEZA FALTANTE: GRAVEDAD ---
        vel.y += 1500.0f * dt; 
    }

    // Limitar velocidad de caída (Terminal Velocity)
    if (vel.y > 1000.0f) vel.y = 1000.0f;

    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    hitbox.x = pos.x;
    hitbox.y = pos.y;
}

void Player::TakeDamage(float amount) {
    if (invulTimer <= 0) {
        health -= amount;
        invulTimer = 1.2f;
        std::cout << "[CORE] Jugador herido. Vida: " << health << std::endl;
    }
}

