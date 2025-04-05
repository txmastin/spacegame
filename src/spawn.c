#include "spawn.h"
#include <stdlib.h>
#include <time.h>

void spawn_stars(Star stars[], int count) {
    for (int i = 0; i < count; i++) {
        stars[i].x = rand() % 1920;
        stars[i].y = rand() % 1080;
    }
}

void spawn_asteroids(Asteroid asteroids[], int count) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < count; i++) {
        asteroids[i].radius = 20 + rand() % 20;
        asteroids[i].x = asteroids[i].radius + rand() % (1920 - (int)(2 * asteroids[i].radius));
        asteroids[i].y = asteroids[i].radius + rand() % (1080 - (int)(2 * asteroids[i].radius));
        asteroids[i].vx = 0.0f;
        asteroids[i].vy = 0.0f;
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
            e->x = rand() % 1920;
            e->y = -20;
            break;
        case 1: // Right edge
            e->x = 1920+20;
            e->y = rand() % 1080;
            break;
        case 2: // Bottom edge
            e->x = rand() % 1920;
            e->y = 1080 + 20;
            break;
        case 3: // Left edge
            e->x = -20;
            e->y = rand() % 1080;
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


