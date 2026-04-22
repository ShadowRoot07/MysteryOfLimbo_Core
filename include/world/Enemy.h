#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

struct Enemy : public Entity {
    EnemyType type;
    float health = 60.0f;
    float timer = 0.0f;
    int dir = 1;

    Enemy() {
        pos = {0, 0};
        vel = {0, 0};
        hitbox = {0, 0, 32, 48};
        type = WALKER;
    }
};

#endif

