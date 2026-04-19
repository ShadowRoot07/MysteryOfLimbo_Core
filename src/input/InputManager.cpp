#include "input/InputManager.h"
#include <cstring>

InputManager::InputManager() : state(nullptr), mouseClicked(false), mouseX(0), mouseY(0) {
    std::memset(lastState, 0, SDL_NUM_SCANCODES);
}

void InputManager::Update() {
    if (state) {
        std::memcpy(lastState, state, SDL_NUM_SCANCODES);
    }
    state = SDL_GetKeyboardState(NULL);
}

void InputManager::HandleRawEvent(SDL_Event& ev) {
    if (ev.type == SDL_MOUSEBUTTONDOWN) {
        mouseClicked = true;
        mouseX = ev.button.x;
        mouseY = ev.button.y;
    }
}

bool InputManager::IsKeyDown(SDL_Scancode key) {
    if (!state) return false;
    return state[key];
}

bool InputManager::IsKeyPressed(SDL_Scancode key) {
    if (!state) return false;
    return state[key] && !lastState[key];
}

