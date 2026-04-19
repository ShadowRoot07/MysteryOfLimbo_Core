#include "input/InputManager.h"
#include <cstring>

void InputManager::Update() {
    // 1. Antes de obtener el nuevo, el estado actual pasa a ser el antiguo
    // Suponiendo que state ya tenía datos del frame anterior
    if (state) {
        std::memcpy(lastState, state, SDL_NUM_SCANCODES);
    } else {
        std::memset(lastState, 0, SDL_NUM_SCANCODES);
    }

    // 2. Obtener el estado real del hardware ahora mismo
    state = SDL_GetKeyboardState(NULL);
}

