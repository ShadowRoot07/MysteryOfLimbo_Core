#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include <map>

class InputManager {
public:
    void Update();
    bool IsKeyDown(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key); // Para saltos (solo una vez)
private:
    const Uint8* state;
    Uint8 lastState[SDL_NUM_SCANCODES];
};

#endif

