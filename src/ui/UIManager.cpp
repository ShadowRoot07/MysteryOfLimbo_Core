#include "ui/UIManager.h"
#include "input/InputManager.h"
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
    // Usando el namespace Assets de Common.h
    texBase = LoadWithTransparency(Assets::JOY_BASE, renderer);
    texKnob = LoadWithTransparency(Assets::JOY_KNOB, renderer);
    texBtnZ = LoadWithTransparency(Assets::BTN_Z, renderer);
    texBtnX = LoadWithTransparency(Assets::BTN_X, renderer);
    texBtnF = LoadWithTransparency(Assets::BTN_F, renderer);

    return (texBase && texKnob && texBtnZ && texBtnX && texBtnF);
}

void UIManager::Render(SDL_Renderer* renderer, const InputManager& input) {
    SDL_Rect areaJoy = {30, 370, 180, 180};
    SDL_RenderCopy(renderer, texBase, NULL, &areaJoy);

    SDL_Rect knobRect = {
        (int)(areaJoy.x + 65 + (input.GetJoystick().x * 50)),
        (int)(areaJoy.y + 65 + (input.GetJoystick().y * 50)),
        50, 50
    };
    SDL_RenderCopy(renderer, texKnob, NULL, &knobRect);

    SDL_Rect rZ = {680, 460, 80, 80}, rX = {590, 480, 80, 80}, rF = {680, 370, 80, 80};
    SDL_RenderCopy(renderer, texBtnZ, NULL, &rZ);
    SDL_RenderCopy(renderer, texBtnX, NULL, &rX);
    SDL_RenderCopy(renderer, texBtnF, NULL, &rF);
}

void UIManager::Clean() {
    if (texBase) SDL_DestroyTexture(texBase);
    if (texKnob) SDL_DestroyTexture(texKnob);
    if (texBtnZ) SDL_DestroyTexture(texBtnZ);
    if (texBtnX) SDL_DestroyTexture(texBtnX);
    if (texBtnF) SDL_DestroyTexture(texBtnF);
    texBase = texKnob = texBtnZ = texBtnX = texBtnF = nullptr;
}

