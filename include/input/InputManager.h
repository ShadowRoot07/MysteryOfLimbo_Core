#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>

struct JoystickData {
    float x, y;          
    bool isActive;       
    SDL_FingerID fingerID; 
};

class InputManager {
public:
    InputManager();
    void Update();
    void HandleRawEvent(SDL_Event& ev, SDL_Renderer* renderer);

    // --- ÁREAS PÚBLICAS (Sincronizadas con UIManager) ---
    SDL_Rect joystickArea;
    SDL_Rect btnZArea;
    SDL_Rect btnXArea;
    SDL_Rect btnFArea;

    // --- MÉTODOS PÚBLICOS (Para que Player.cpp pueda leer el input) ---
    bool IsKeyDown(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key);
    SDL_Point GetJoystickScreenPos() const;
    JoystickData GetJoystick() const { return joystick; }

private:
    const Uint8* state;
    Uint8 lastState[SDL_NUM_SCANCODES];

    JoystickData joystick;

    // Estados lógicos de los botones virtuales
    bool vJump, vAttack, vDash;
    bool lastVJump, lastVAttack, lastVDash;
};

#endif

