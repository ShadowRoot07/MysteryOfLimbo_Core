#ifndef PLATFORM_H
#define PLATFORM_H

#include "Common.h"

enum PlatformType { NORMAL, SPIKE, BOUNCY };

struct Platform {
    Rect bounds;
    PlatformType type;
    float damageValue;
};

#endif

