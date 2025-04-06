#include "spawn.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#ifndef M_PI
#define M_PI 3.1415926535
#endif

void spawn_stars(Star stars[], int count) {
    for (int i = 0; i < count; i++) {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
    }
}

void spawn_asteroid(Asteroid* a) {
    int edge = rand() % 4;
    float buffer = 40.0f;  // spawn just offscreen

    switch (edge) {
        case 0: // left
            a->x = -buffer;
            a->y = rand() % SCREEN_HEIGHT;
            break;
        case 1: // right
            a->x = SCREEN_WIDTH + buffer;
            a->y = rand() % SCREEN_HEIGHT;
            break;
        case 2: // top
            a->x = rand() % SCREEN_WIDTH;
            a->y = -buffer;
            break;
        case 3: // bottom
            a->x = rand() % SCREEN_WIDTH;
            a->y = SCREEN_HEIGHT + buffer;
            break;
    }

    a->radius = 20 + rand() % 20;
    float angle = (rand() % 360) * M_PI / 180.0f;
    float speed = 0.2f + (rand() % (60 - (int)(a->radius))) / 100.0f;
    a->vx = cosf(angle) * speed;
    a->vy = sinf(angle) * speed;

    a->hits_required = (int)a->radius / 6 + 2;
    a->hits_taken = 0;
    a->alive = 1;
    a->being_mined = 0;
}

void spawn_asteroids(Asteroid asteroids[], int count) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < (count-7); i++) {
        asteroids[i].radius = 20 + rand() % 20;
        asteroids[i].x = asteroids[i].radius + rand() % (SCREEN_WIDTH - (int)(2 * asteroids[i].radius));
        asteroids[i].y = asteroids[i].radius + rand() % (SCREEN_HEIGHT - (int)(2 * asteroids[i].radius));
        asteroids[i].vx = 0.0f;
        asteroids[i].vy = 0.0f;
        float angle = (rand() % 360) * M_PI / 180.0f;
        float speed = 0.2f + (rand() % (60 - (int)(asteroids[i].radius))) / 100.0f;
        asteroids[i].vx = cosf(angle) * speed;
        asteroids[i].vy = sinf(angle) * speed;
        asteroids[i].alive = 1;
        asteroids[i].hits_taken = 0;
        asteroids[i].hits_required = (int)asteroids[i].radius / 4 + 2;
        asteroids[i].being_mined = 0;
    }
}

void spawn_enemy(Enemy* e) {
    int side = rand() % 4; // 0 = top, 1 = right, 2 = bottom, 3 = left
    switch (side) {
        case 0: // Top edge
            e->x = rand() % SCREEN_WIDTH;
            e->y = -20;
            break;
        case 1: // Right edge
            e->x = SCREEN_WIDTH+20;
            e->y = rand() % SCREEN_HEIGHT;
            break;
        case 2: // Bottom edge
            e->x = rand() % SCREEN_WIDTH;
            e->y = SCREEN_HEIGHT + 20;
            break;
        case 3: // Left edge
            e->x = -20;
            e->y = rand() % SCREEN_HEIGHT;
            break;
    } 
    e->angle = 0.0f;
    e->vx = 0;
    e->vy = 0;
    e->speed = 0.5f + ((float)(rand() % 200) / 100.f);
    e->last_shot_time = SDL_GetTicks();
    e->alive = 1;
    e->jitter_angle = (float)(rand() % 628) / 100.0f;
}


void spawn_enemies(Enemy enemies[], int count) {
    for (int i = 0; i < count; i++) {
        enemies[i].x = rand() % 1800 + 60;
        enemies[i].y = rand() % 1000 + 40;
        enemies[i].vx = 0;
        enemies[i].vy = 0;
        enemies[i].speed = 1.5f;
        enemies[i].last_shot_time = SDL_GetTicks();
        enemies[i].alive = 1;
        enemies[i].jitter_angle = (float)(rand() % 628) / 100.0f;
    }
}


