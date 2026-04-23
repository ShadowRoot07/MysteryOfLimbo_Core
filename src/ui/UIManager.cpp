#include "ui/UIManager.h"
#include "input/InputManager.h"
#include "player/Player.h" // Necesario para acceder al estado del jugador
#include "Common.h"
#include <iostream>

UIManager::UIManager() : texBase(nullptr), texKnob(nullptr), texBtnZ(nullptr), texBtnX(nullptr), texBtnF(nullptr) {}

UIManager::~UIManager() { Clean(); }

SDL_Texture* UIManager::LoadWithTransparency(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = SDL_LoadBMP(path);
    if (!surface) {
        std::cerr << "Error cargando BMP: " << SDL_GetError() << " -> " << path << std::endl;
        return nullptr;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

bool UIManager::LoadAssets(SDL_Renderer* renderer) {
    texBase = LoadWithTransparency(Assets::JOY_BASE, renderer);
    texKnob = LoadWithTransparency(Assets::JOY_KNOB, renderer);
    texBtnZ = LoadWithTransparency(Assets::BTN_Z, renderer);
    texBtnX = LoadWithTransparency(Assets::BTN_X, renderer);
    texBtnF = LoadWithTransparency(Assets::BTN_F, renderer);

    return (texBase && texKnob && texBtnZ && texBtnX && texBtnF);
}

// Ahora recibe Player para mostrar los elementos equipados
void UIManager::Render(SDL_Renderer* renderer, const InputManager& input, const Player& player) {
    // 1. Dibujar Joystick Virtual
    SDL_Rect areaJoy = {30, 370, 180, 180};
    SDL_RenderCopy(renderer, texBase, NULL, &areaJoy);

    SDL_Rect knobRect = {
        (int)(areaJoy.x + 65 + (input.GetJoystick().x * 50)),
        (int)(areaJoy.y + 65 + (input.GetJoystick().y * 50)),
        50, 50
    };
    SDL_RenderCopy(renderer, texKnob, NULL, &knobRect);

    // 2. Dibujar Botones de Acción
    SDL_Rect rZ = {680, 460, 80, 80}, rX = {590, 480, 80, 80}, rF = {680, 370, 80, 80};
    SDL_RenderCopy(renderer, texBtnZ, NULL, &rZ);
    SDL_RenderCopy(renderer, texBtnX, NULL, &rX);
    SDL_RenderCopy(renderer, texBtnF, NULL, &rF);

    // 3. INDICADORES DE ELEMENTOS (Debug Visual)
    // Dibujamos pequeños rectángulos en la parte superior para saber qué elementos hay
    if (player.HasElement(AIR)) {
        SDL_SetRenderDrawColor(renderer, 150, 255, 255, 255); // Cyan para Aire
        SDL_Rect r = {10, 10, 20, 20}; SDL_RenderFillRect(renderer, &r);
    }
    if (player.HasElement(LIGHT)) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255); // Amarillo para Luz
        SDL_Rect r = {40, 10, 20, 20}; SDL_RenderFillRect(renderer, &r);
    }
    if (player.HasElement(WATER)) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255); // Azul para Agua
        SDL_Rect r = {70, 10, 20, 20}; SDL_RenderFillRect(renderer, &r);
    }
}

void UIManager::Clean() {
    if (texBase) SDL_DestroyTexture(texBase);
    if (texKnob) SDL_DestroyTexture(texKnob);
    if (texBtnZ) SDL_DestroyTexture(texBtnZ);
    if (texBtnX) SDL_DestroyTexture(texBtnX);
    if (texBtnF) SDL_DestroyTexture(texBtnF);
    texBase = texKnob = texBtnZ = texBtnX = texBtnF = nullptr;
}

