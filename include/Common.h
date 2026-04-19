#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>

struct Vector2 {
    float x, y;
};

struct Rect {
    float x, y, w, h;
};

struct Entity {
    Vector2 pos;
    Vector2 vel;
    Rect hitbox;
    bool isGrounded;
};

// Rutas de recursos para fácil acceso
namespace Assets {
    const char* const JOY_BASE = "assets/sprites/ui/joystick_base.bmp";
    const char* const JOY_KNOB = "assets/sprites/ui/joystick_knob.bmp";
    const char* const BTN_Z     = "assets/sprites/ui/button_z.bmp";
}

#endif

