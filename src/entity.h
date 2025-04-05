#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

typedef struct {
    float x, y;
} Star;

// ---- Player Ship ----
typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    int width, height;
    float thrust;
    float damping;
    float angle;
} PlayerShip;

// ---- Asteroid ----
typedef struct {
    float x, y;
    float vx, vy;
    float radius;
    int hits_required;
    int hits_taken;
    int alive;
    int being_mined;
} Asteroid;

// ---- Projectile ----
typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    int alive;
    int owner;
} Projectile;

// ---- Enemy ----
typedef struct {
    float x, y;
    float vx, vy;
    float angle;
    float speed;
    Uint32 last_shot_time;
    int alive;
    float jitter_angle;
} Enemy;

#endif

