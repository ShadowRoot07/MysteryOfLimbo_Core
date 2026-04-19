#include "player/Player.h"
#include <iostream>
#include <algorithm>

Player::Player() {
    pos = {100.0f, 100.0f};
    vel = {0.0f, 0.0f};
    hitbox = {0, 0, 32, 48}; // Tamaño estándar
    isGrounded = false;
    
    health = 100.0f;
    maxHealth = 100.0f;
    speed = 300.0f;
    jumpForce = -550.0f; // Negativo porque en SDL Y aumenta hacia abajo
    
    dashCooldown = 0.0f;
    dashTimer = 0.0f;
    isDashing = false;
    invulTimer = 0.0f;
}

void Player::HandleInput(InputManager& input) {
    if (isDashing) return; // No se puede mover manualmente durante el dash

    vel.x = 0;

    if (input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        vel.x = -speed;
    }
    if (input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        vel.x = speed;
    }

    // Salto (Solo si está en el suelo)
    if (input.IsKeyPressed(SDL_SCANCODE_Z) && isGrounded) {
        vel.y = jumpForce;
        isGrounded = false;
    }

    // Dash (Tecla X)
    if (input.IsKeyPressed(SDL_SCANCODE_X) && dashCooldown <= 0) {
        float dir = (vel.x >= 0) ? 1.0f : -1.0f;
        ApplyDash(dir);
    }
}

void Player::ApplyDash(float direction) {
    isDashing = true;
    dashTimer = 0.2f;      // Duración del dash
    dashCooldown = 0.8f;   // Tiempo hasta el próximo dash
    vel.x = direction * speed * 3.0f;
    vel.y = 0;             // El dash suspende la gravedad momentáneamente
}

void Player::Update(float dt) {
    // Actualizar temporizadores
    if (invulTimer > 0) invulTimer -= dt;
    if (dashCooldown > 0) dashCooldown -= dt;

    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) isDashing = false;
    }

    // Actualizar posición basado en velocidad (Físicas básicas)
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    // Actualizar la hitbox para que siga a la posición
    hitbox.x = pos.x;
    hitbox.y = pos.y;
}

void Player::TakeDamage(float amount) {
    if (invulTimer <= 0) {
        health -= amount;
        invulTimer = 1.2f; // Breve periodo de gracia
        std::cout << "[CORE] Jugador herido. Vida: " << health << std::endl;
        
        if (health <= 0) {
            std::cout << "[CORE] GAME OVER" << std::endl;
            // Aquí podrías resetear la posición del jugador
        }
    }
}

