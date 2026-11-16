#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdbool.h>

typedef struct {
    float x, y;
    float speed; /* px/s */
    int lives;
    int score;
    int limit_x; /* metade da largura (colisão) */
    int limit_y; /* metade da altura  (colisão) */
    float damage_anim;
} SpaceShip;

typedef struct {
    float x, y;
    bool active; /* o tiro está em voo? */
    float speed;
    float size;
} Bullet;

typedef struct {
    float x, y;
    bool active;
    float speed;
    int limit_x;
    int limit_y;
} Comet;

typedef struct {
    float x, y;
    bool active;
    float speed;
    int limit_x;
    int limit_y;
    int Dir;
} Missile;

#endif
