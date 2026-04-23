#include "ui/UIManager.h"
#include "player/Player.h" // Incluido para acceder a GetHealth, GetPos, etc.
#include <iostream>

UIManager::UIManager() : btnF(nullptr), btnX(nullptr), btnZ(nullptr), joyBase(nullptr), joyKnob(nullptr) {}

SDL_Texture* UIManager::LoadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = SDL_LoadBMP(path);
    if (!tempSurface) {
        std::cerr << "Fallo al cargar imagen: " << path << " Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

bool UIManager::LoadAssets(SDL_Renderer* renderer) {
    btnF = LoadTexture("assets/sprites/ui/botonF.bmp", renderer);
    btnX = LoadTexture("assets/sprites/ui/botonX.bmp", renderer);
    btnZ = LoadTexture("assets/sprites/ui/button_z.bmp", renderer);
    joyBase = LoadTexture("assets/sprites/ui/joystick_base.bmp", renderer);
    joyKnob = LoadTexture("assets/sprites/ui/joystick_knob.bmp", renderer);

    // Retorna true solo si todos los assets críticos se cargaron
    return (btnF && btnX && btnZ && joyBase && joyKnob);
}

void UIManager::Render(SDL_Renderer* renderer, const InputManager& input, const Player& player) {
    // 1. Renderizar Botones (Sincronizados con las áreas táctiles del InputManager)
    // Áreas en InputManager: Z(680,460), X(590,480), F(680,370) - tamaño 80x80
    SDL_Rect rectZ = { 680, 460, 80, 80 };
    SDL_Rect rectX = { 590, 480, 80, 80 };
    SDL_Rect rectF = { 680, 370, 80, 80 };

    SDL_RenderCopy(renderer, btnZ, NULL, &rectZ);
    SDL_RenderCopy(renderer, btnX, NULL, &rectX);
    SDL_RenderCopy(renderer, btnF, NULL, &rectF);

    // 2. Renderizar Joystick (Sincronizado con joystickArea {30, 370, 180, 180})
    // Llamamos al método correcto: GetJoystickScreenPos()
    SDL_Point jPos = input.GetJoystickScreenPos();
    
    SDL_Rect baseRect = { 30, 370, 180, 180 };
    // El knob (perilla) se centra en la posición calculada por el InputManager
    SDL_Rect knobRect = { jPos.x - 40, jPos.y - 40, 80, 80 };

    SDL_RenderCopy(renderer, joyBase, NULL, &baseRect);
    SDL_RenderCopy(renderer, joyKnob, NULL, &knobRect);

    // 3. Feedback de Vida (Basado en el player)
    // Fondo de la barra
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect healthBg = { 20, 20, 200, 20 };
    SDL_RenderFillRect(renderer, &healthBg);

    // Barra de salud roja
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // Asumiendo que player.GetHealth() retorna un valor que escalado da el ancho
    SDL_Rect healthBar = { 20, 20, (int)(player.GetHealth() * 2), 20 };
    SDL_RenderFillRect(renderer, &healthBar);
}

void UIManager::Clean() {
    if (btnF) SDL_DestroyTexture(btnF);
    if (btnX) SDL_DestroyTexture(btnX);
    if (btnZ) SDL_DestroyTexture(btnZ);
    if (joyBase) SDL_DestroyTexture(joyBase);
    if (joyKnob) SDL_DestroyTexture(joyKnob);
}

