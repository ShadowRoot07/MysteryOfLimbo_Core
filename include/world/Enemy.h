#ifndef ENEMY_H
#define ENEMY_H
#include "Common.h"

struct Enemy : public Entity {
    EnemyType type;
    float health = 60.0f; // 3 golpes de 25
    float timer = 0;
    int dir = 1;
};
#endif

