#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>

struct JoystickData {
    float x, y;
    bool isActive;
    SDL_FingerID fingerID; // Rastreo de dedo para multitouch
};

class InputManager {
public:
    InputManager();
    void Update();
    void HandleRawEvent(SDL_Event& ev);

    bool IsKeyDown(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key);

    JoystickData GetJoystick() const { return joystick; }
    bool IsJumpButtonPressed() const { return virtualJump; }

private:
    const Uint8* state;
    Uint8 lastState[SDL_NUM_SCANCODES];

    JoystickData joystick;
    bool virtualJump;
    SDL_FingerID jumpFingerID; // ID del dedo que presiona saltar

    SDL_Rect joystickArea;
    SDL_Rect jumpButtonArea;
};

#endif

