// include/Common.h
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

#endif

