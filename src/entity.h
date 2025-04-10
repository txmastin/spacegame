#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>


/****** Stars ******/
typedef struct {
    float x, y;
} Star;

typedef struct {
    float x, y;
    float speed;
    float length;
} StreakStar;

/****** Player Ship ******/
typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    int width, height;
    float thrust;
    float damping;
    float angle;
    int material_collected;
    int pirates_eliminated;
} PlayerShip;

/****** Asteroid ******/

#define ASTEROID_MAX_VERTICES 16

typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    int hits_required;
    int hits_taken;
    int alive;
    int vertex_count;
    SDL_FPoint outline[ASTEROID_MAX_VERTICES];
    int being_mined;
} Asteroid;

/****** Projectile ******/
typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    int alive;
    int owner;
} Projectile;


/****** Enemy ******/
typedef enum {
    ENEMY_BASIC,
    ENEMY_SPIRAL
} EnemyType;

typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    float speed;
    Uint32 last_shot_time;
    int alive;
    float jitter_angle;
    EnemyType type;
} Enemy;

#endif

