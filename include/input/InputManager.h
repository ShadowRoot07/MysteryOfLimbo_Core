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
    void HandleRawEvent(SDL_Event& ev);

    bool IsKeyDown(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key);

    JoystickData GetJoystick() const { return joystick; }

private:
    const Uint8* state;
    Uint8 lastState[SDL_NUM_SCANCODES];

    JoystickData joystick;
    
    // Estados de botones virtuales
    bool vJump;
    bool vAttack;
    bool vDash;

    // Áreas de botones táctiles
    SDL_Rect joystickArea;
    SDL_Rect btnZArea;
    SDL_Rect btnXArea;
    SDL_Rect btnFArea;
};

#endif

