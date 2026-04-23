#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SDL.h>
#include "input/InputManager.h"

// Forward Declaration: Avisamos que la clase Player existe
// sin necesidad de incluir su header aquí, evitando ciclos.
class Player;

class UIManager {
public:
    UIManager();
    bool LoadAssets(SDL_Renderer* renderer);
    // Ahora el compilador acepta Player& porque sabe que es una clase.
    void Render(SDL_Renderer* renderer, const InputManager& input, const Player& player);
    void Clean();

private:
    SDL_Texture* btnF;
    SDL_Texture* btnX;
    SDL_Texture* btnZ;
    SDL_Texture* joyBase;
    SDL_Texture* joyKnob;

    SDL_Texture* LoadTexture(const char* path, SDL_Renderer* renderer);
};

#endif

