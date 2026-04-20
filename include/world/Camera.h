#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"

struct Camera {
    Vector2 pos;
    int w, h;

    Camera(int width, int height) : w(width), h(height) {
        pos = {0, 0};
    }

    // Centra la cámara en el jugador de forma suave
    void Follow(Vector2 targetPos, float dt) {
        float targetX = targetPos.x - w / 2;
        float targetY = targetPos.y - h / 2;
        
        // Efecto de suavizado (Lerp)
        pos.x += (targetX - pos.x) * 5.0f * dt;
        pos.y += (targetY - pos.y) * 5.0f * dt;
    }
};

#endif

