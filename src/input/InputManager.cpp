#include "input/InputManager.h"
#include <cstring>
#include <cmath>

InputManager::InputManager() {
    state = nullptr;
    std::memset(lastState, 0, SDL_NUM_SCANCODES);
    joystick = {0.0f, 0.0f, false, -1};

    joystickArea = {30, 370, 180, 180};
    btnZArea = {680, 460, 80, 80};
    btnXArea = {590, 480, 80, 80};
    btnFArea = {680, 370, 80, 80};

    vJump = vAttack = vDash = false;
    lastVJump = lastVAttack = lastVDash = false;
}

void InputManager::Update() {
    if (state) std::memcpy(lastState, state, SDL_NUM_SCANCODES);
    lastVJump = vJump;
    lastVAttack = vAttack;
    lastVDash = vDash;
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
                // Centro del joystick
                float centerX = joystickArea.x + 90.0f;
                float centerY = joystickArea.y + 90.0f;
                // Vector normalizado (rango -1.0 a 1.0)
                joystick.x = (mx - centerX) / 90.0f;
                joystick.y = (my - centerY) / 90.0f;
                
                // Clamp para no exceder 1.0
                if(joystick.x > 1.0f) joystick.x = 1.0f; if(joystick.x < -1.0f) joystick.x = -1.0f;
                if(joystick.y > 1.0f) joystick.y = 1.0f; if(joystick.y < -1.0f) joystick.y = -1.0f;
            }
            if (SDL_PointInRect(&p, &btnZArea)) vJump = true;
            if (SDL_PointInRect(&p, &btnXArea)) vAttack = true;
            if (SDL_PointInRect(&p, &btnFArea)) vDash = true;
        }

        if (ev.type == SDL_FINGERUP) {
            if (fid == joystick.fingerID) { joystick.isActive = false; joystick.x = 0; joystick.y = 0; }
            if (SDL_PointInRect(&p, &btnZArea)) vJump = false;
            if (SDL_PointInRect(&p, &btnXArea)) vAttack = false;
            if (SDL_PointInRect(&p, &btnFArea)) vDash = false;
        }
    }
}

bool InputManager::IsKeyDown(SDL_Scancode k) {
    if (k == SDL_SCANCODE_LEFT && joystick.x < -0.3f) return true;
    if (k == SDL_SCANCODE_RIGHT && joystick.x > 0.3f) return true;
    if (k == SDL_SCANCODE_UP && joystick.y < -0.3f) return true;
    if (k == SDL_SCANCODE_DOWN && joystick.y > 0.3f) return true;
    return (state && state[k]);
}

bool InputManager::IsKeyPressed(SDL_Scancode k) {
    if (k == SDL_SCANCODE_Z) return (vJump && !lastVJump) || (state && state[k] && !lastState[k]);
    if (k == SDL_SCANCODE_X) return (vAttack && !lastVAttack) || (state && state[k] && !lastState[k]);
    if (k == SDL_SCANCODE_F) return (vDash && !lastVDash) || (state && state[k] && !lastState[k]);
    return (state && state[k] && !lastState[k]);
}

