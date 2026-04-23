#ifndef PLATFORM_H
#define PLATFORM_H

#include "Common.h"

enum PlatformType { NORMAL, SPIKE, TEMPORARY };

struct Platform {
    Rect bounds;
    PlatformType type;
    float lifetime; // Tiempo restante si es TEMPORARY
};

#endif

