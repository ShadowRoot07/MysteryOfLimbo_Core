#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>

struct Vector2 { float x, y; };
struct Rect { float x, y, w, h; };

// Elementos disponibles en el juego
enum ElementType { FIRE, AIR, LIGHT, EARTH, WATER, DARKNESS, NONE };

struct Entity {
    Vector2 pos;
    Vector2 vel;
    Rect hitbox;
    bool isGrounded;
};

enum EnemyType { WALKER, FLYER, TURRET };

struct Projectile {
    Vector2 pos;
    Vector2 vel;
    Rect hitbox;
    bool active;
};

namespace Assets {
    const char* const JOY_BASE = "assets/sprites/ui/joystick_base.bmp";
    const char* const JOY_KNOB = "assets/sprites/ui/joystick_knob.bmp";
    const char* const BTN_Z    = "assets/sprites/ui/button_z.bmp";
    const char* const BTN_X    = "assets/sprites/ui/botonX.bmp";
    const char* const BTN_F    = "assets/sprites/ui/botonF.bmp";
}

#endif

