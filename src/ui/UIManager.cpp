#include "ui/UIManager.h"
#include "player/Player.h"
#include "Common.h" // Para acceder a Assets::

UIManager::UIManager() {}

bool UIManager::LoadAssets(ShadowGFX& gfx) {
    // Registramos usando las constantes de Common.h
    gfx.GetTexture("btnF", Assets::BTN_F);
    gfx.GetTexture("btnX", Assets::BTN_X);
    gfx.GetTexture("btnZ", Assets::BTN_Z);
    gfx.GetTexture("joyBase", Assets::JOY_BASE);
    gfx.GetTexture("joyKnob", Assets::JOY_KNOB);
    return true; 
}

void UIManager::Render(SDL_Renderer* renderer, ShadowGFX& gfx, const InputManager& input, const Player& player) {
    // Dibujamos las texturas
    gfx.DrawStatic("btnZ", input.btnZArea);
    gfx.DrawStatic("btnX", input.btnXArea);
    gfx.DrawStatic("btnF", input.btnFArea);
    gfx.DrawStatic("joyBase", input.joystickArea);

    SDL_Point jPos = input.GetJoystickScreenPos();
    SDL_Rect knobRect = { jPos.x - 40, jPos.y - 40, 80, 80 };
    gfx.DrawStatic("joyKnob", knobRect);

    // --- OPCIÓN 5: DEBUG VISUAL DE COLISIÓN ---
    // Esto dibujará marcos rojos sobre las áreas de toque reales
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_RenderDrawRect(renderer, &input.btnZArea);
    SDL_RenderDrawRect(renderer, &input.btnXArea);
    SDL_RenderDrawRect(renderer, &input.btnFArea);
    SDL_RenderDrawRect(renderer, &input.joystickArea);
    
    // Dibujar un pequeño punto donde el InputManager cree que está el centro del joystick
    SDL_RenderDrawLine(renderer, input.joystickArea.x, input.joystickArea.y, 
                       input.joystickArea.x + input.joystickArea.w, input.joystickArea.y + input.joystickArea.h);

    // Barra de vida
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect healthBg = { 20, 20, 200, 20 };
    SDL_RenderFillRect(renderer, &healthBg);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthBar = { 20, 20, (int)(player.GetHealth() * 2), 20 };
    SDL_RenderFillRect(renderer, &healthBar);
}

void UIManager::Clean() {}

