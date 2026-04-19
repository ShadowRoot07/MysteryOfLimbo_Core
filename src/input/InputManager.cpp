#include "input/InputManager.h"
#include <cstring>

InputManager::InputManager() : state(nullptr), virtualJump(false), jumpFingerID(-1) {
    std::memset(lastState, 0, SDL_NUM_SCANCODES);
    joystick = {0.0f, 0.0f, false, -1};
    joystickArea = {50, 400, 200, 200};
    jumpButtonArea = {650, 450, 100, 100};
}

void InputManager::Update() {
    if (state) std::memcpy(lastState, state, SDL_NUM_SCANCODES);
    state = SDL_GetKeyboardState(NULL);
}

void InputManager::HandleRawEvent(SDL_Event& ev) {
    // Usamos eventos de dedo para MULTITOUCH real
    if (ev.type == SDL_FINGERDOWN || ev.type == SDL_FINGERMOTION || ev.type == SDL_FINGERUP) {
        // SDL_Finger entrega coordenadas de 0.0 a 1.0
        int mx = (int)(ev.tfinger.x * 800);
        int my = (int)(ev.tfinger.y * 600);
        SDL_Point p = {mx, my};
        SDL_FingerID fid = ev.tfinger.fingerId;

        if (ev.type == SDL_FINGERDOWN || ev.type == SDL_FINGERMOTION) {
            // Lógica de Joystick independiente
            if (SDL_PointInRect(&p, &joystickArea)) {
                if (!joystick.isActive || joystick.fingerID == fid) {
                    joystick.isActive = true;
                    joystick.fingerID = fid;
                    float centerX = joystickArea.x + joystickArea.w / 2.0f;
                    joystick.x = (mx - centerX) / (joystickArea.w / 2.0f);
                }
            }
            // Lógica de Salto independiente
            if (SDL_PointInRect(&p, &jumpButtonArea)) {
                virtualJump = true;
                jumpFingerID = fid;
            }
        } 
        
        if (ev.type == SDL_FINGERUP) {
            if (fid == joystick.fingerID) {
                joystick.isActive = false;
                joystick.x = 0.0f;
                joystick.fingerID = -1;
            }
            if (fid == jumpFingerID) {
                virtualJump = false;
                jumpFingerID = -1;
            }
        }
    }
}

bool InputManager::IsKeyDown(SDL_Scancode key) {
    if (key == SDL_SCANCODE_LEFT && joystick.x < -0.3f) return true;
    if (key == SDL_SCANCODE_RIGHT && joystick.x > 0.3f) return true;
    return (state && state[key]);
}

bool InputManager::IsKeyPressed(SDL_Scancode key) {
    if (key == SDL_SCANCODE_Z && virtualJump) return true;
    return (state && state[key] && !lastState[key]);
}

