#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SDL.h>
#include "input/InputManager.h"

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool LoadAssets(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer, const InputManager& input, const Player& player);
    void Clean();

private:
    SDL_Texture* texBase;
    SDL_Texture* texKnob;
    SDL_Texture* texBtnZ;
    SDL_Texture* texBtnX;
    SDL_Texture* texBtnF;

    SDL_Texture* LoadWithTransparency(const char* path, SDL_Renderer* renderer);
};

#endif

