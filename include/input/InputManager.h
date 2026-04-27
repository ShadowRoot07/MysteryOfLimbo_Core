#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>

// Estructura para manejar los datos del joystick táctil
struct JoystickData {
    float x, y;          // Valores normalizados de -1.0 a 1.0
    bool isActive;       // Indica si el usuario tiene el dedo sobre el joystick
    SDL_FingerID fingerID; // Identificador del dedo para evitar conflictos multitouch
};

class InputManager {
public:
    InputManager();
    
    // Actualiza los estados de las teclas (teclado y botones virtuales)
    void Update();
    
    // Procesa eventos SDL_tfinger (Touch) y eventos del sistema
    void HandleRawEvent(SDL_Event& ev);

    // Verificación de estados
    bool IsKeyDown(SDL_Scancode key);
    bool IsKeyPressed(SDL_Scancode key);

    // Retorna la estructura con los datos lógicos del joystick
    JoystickData GetJoystick() const { return joystick; }

    // Retorna la posición en coordenadas de pantalla (píxeles) para que la UI dibuje la perilla
    SDL_Point GetJoystickScreenPos() const;

private:
    const Uint8* state;                   // Estado actual del teclado
    Uint8 lastState[SDL_NUM_SCANCODES];   // Estado del frame anterior (para IsKeyPressed)

    JoystickData joystick;

    // Estados lógicos de los botones virtuales (Mapeados a Z, X, F)
    bool vJump, vAttack, vDash;
    bool lastVJump, lastVAttack, lastVDash;

    // Áreas de colisión táctil (Sincronizadas con el renderizado en UIManager)
    SDL_Rect joystickArea;
    SDL_Rect btnZArea;
    SDL_Rect btnXArea;
    SDL_Rect btnFArea;
};

#endif

