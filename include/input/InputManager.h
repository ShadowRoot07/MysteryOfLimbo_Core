#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>

class InputManager {
public:
    InputManager();
    void Update();
    void HandleRawEvent(SDL_Event& ev); // Nuevo: procesa clics/toques
    
    bool IsKeyDown(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key);
    
    // Getters para el Debug Tactil
    bool WasMouseClicked() const { return mouseClicked; }
    int GetMouseX() const { return mouseX; }
    int GetMouseY() const { return mouseY; }
    void ResetMouseClick() { mouseClicked = false; }

private:
    const Uint8* state;
    Uint8 lastState[SDL_NUM_SCANCODES];
    
    bool mouseClicked;
    int mouseX, mouseY;
};

#endif

