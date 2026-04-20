#include "input/InputManager.h"
#include <cstring>

InputManager::InputManager() {
    state = nullptr;
    std::memset(lastState, 0, SDL_NUM_SCANCODES);
    joystick = {0.0f, 0.0f, false, -1};
    
    // Áreas de botones (Ajustadas para tu ZTE)
    joystickArea = {30, 370, 180, 180};
    btnZArea = {680, 460, 80, 80}; // Salto
    btnXArea = {590, 480, 80, 80}; // Ataque
    btnFArea = {680, 370, 80, 80}; // Dash
    
    vJump = vAttack = vDash = false;
}

void InputManager::Update() {
    if (state) std::memcpy(lastState, state, SDL_NUM_SCANCODES);
    state = SDL_GetKeyboardState(NULL);
}

void InputManager::HandleRawEvent(SDL_Event& ev) {
    if (ev.type == SDL_FINGERDOWN || ev.type == SDL_FINGERMOTION || ev.type == SDL_FINGERUP) {
        int mx = (int)(ev.tfinger.x * 800);
        int my = (int)(ev.tfinger.y * 600);
        SDL_Point p = {mx, my};
        SDL_FingerID fid = ev.tfinger.fingerId;

        if (ev.type == SDL_FINGERDOWN || ev.type == SDL_FINGERMOTION) {
            if (SDL_PointInRect(&p, &joystickArea)) {
                joystick.isActive = true;
                joystick.fingerID = fid;
                joystick.x = (mx - (joystickArea.x + 90)) / 90.0f;
            }
            if (SDL_PointInRect(&p, &btnZArea)) vJump = true;
            if (SDL_PointInRect(&p, &btnXArea)) vAttack = true;
            if (SDL_PointInRect(&p, &btnFArea)) vDash = true;
        }

        if (ev.type == SDL_FINGERUP) {
            if (fid == joystick.fingerID) { joystick.isActive = false; joystick.x = 0; }
            if (SDL_PointInRect(&p, &btnZArea)) vJump = false;
            if (SDL_PointInRect(&p, &btnXArea)) vAttack = false;
            if (SDL_PointInRect(&p, &btnFArea)) vDash = false;
        }
    }
}

bool InputManager::IsKeyDown(SDL_Scancode k) {
    if (k == SDL_SCANCODE_LEFT && joystick.x < -0.3f) return true;
    if (k == SDL_SCANCODE_RIGHT && joystick.x > 0.3f) return true;
    return (state && state[k]);
}

bool InputManager::IsKeyPressed(SDL_Scancode k) {
    if (k == SDL_SCANCODE_Z && vJump) return true;
    if (k == SDL_SCANCODE_X && vAttack) return true;
    if (k == SDL_SCANCODE_F && vDash) return true;
    return (state && state[k] && !lastState[k]);
}

