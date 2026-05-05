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
    // ELIMINADA: La llamada a SDL_RenderSetLogicalSize aquí. 
    // Ya está definida globalmente en main.cpp.

    // Dibujamos usando las áreas oficiales del InputManager (Sincronización Total)
    gfx.DrawStatic("btnZ", input.btnZArea);
    gfx.DrawStatic("btnX", input.btnXArea);
    gfx.DrawStatic("btnF", input.btnFArea);
    gfx.DrawStatic("joyBase", input.joystickArea);

    // Knob centrado perfectamente
    SDL_Point jPos = input.GetJoystickScreenPos();
    SDL_Rect knobRect = { jPos.x - 40, jPos.y - 40, 80, 80 };
    gfx.DrawStatic("joyKnob", knobRect);

    // Barra de vida
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect healthBg = { 20, 20, 200, 20 };
    SDL_RenderFillRect(renderer, &healthBg);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthBar = { 20, 20, (int)(player.GetHealth() * 2), 20 };
    SDL_RenderFillRect(renderer, &healthBar);
}

void UIManager::Clean() {}

