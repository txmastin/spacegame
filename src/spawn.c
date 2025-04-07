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

void spawn_streak_stars(StreakStar stars[], int count) {
    for (int i = 0; i < count; i++) {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;

        // 50/50 chance: short (3-5 px) or long (8-12 px)
        int long_streak = rand() % 2;
        if (long_streak) {
            stars[i].length = 5 + rand() % 5;  // 8–12 px
        } else {
            stars[i].length = 3 + rand() % 3;  // 3–5 px
        }

        // Speed is proportional to length
        stars[i].speed = stars[i].length * 0.3f;
    }
}

void generate_asteroid_outline(Asteroid* a) {
    a->vertex_count = ASTEROID_MAX_VERTICES;
    float step = 2 * M_PI / a->vertex_count;

    for (int i = 0; i < a->vertex_count; i++) {
        float angle = i * step;
        float noise = 0.7f + ((rand() % 30) / 100.0f);  // random between 0.7 - 1.0
        float r = a->radius * noise;
        a->outline[i].x = cosf(angle) * r;
        a->outline[i].y = sinf(angle) * r;
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

    a->radius = 20 + rand() % 30;
    generate_asteroid_outline(a);    

    float angle = (rand() % 360) * M_PI / 180.0f;
    float speed = 0.2f + (rand() % (70 - (int)(a->radius))) / 100.0f;
    a->vx = cosf(angle) * speed;
    a->vy = sinf(angle) * speed;

    a->hits_required = (int)a->radius / 6;
    a->hits_taken = 0;
    a->alive = 1;
    a->being_mined = 0;
}

void spawn_asteroids(Asteroid asteroids[], int count) {
    srand((unsigned int)time(NULL));

    for (int i = 0; i < (count-10); i++) {
        asteroids[i].radius = 20 + rand() % 30;
        generate_asteroid_outline(&asteroids[i]);
        asteroids[i].x = asteroids[i].radius + rand() % (SCREEN_WIDTH - (int)(2 * asteroids[i].radius));
        asteroids[i].y = asteroids[i].radius + rand() % (SCREEN_HEIGHT - (int)(2 * asteroids[i].radius));
        asteroids[i].vx = 0.0f;
        asteroids[i].vy = 0.0f;
        float angle = (rand() % 360) * M_PI / 180.0f;
        float speed = 0.2f + (rand() % (70 - (int)(asteroids[i].radius))) / 100.0f;
        asteroids[i].vx = cosf(angle) * speed;
        asteroids[i].vy = sinf(angle) * speed;
        asteroids[i].alive = 1;
        asteroids[i].hits_taken = 0;
        asteroids[i].hits_required = (int)asteroids[i].radius / 6;
        asteroids[i].being_mined = 0;
    }
}

void spawn_enemy(Enemy* e) {
    int side = rand() % 4; // 0 = top, 1 = right, 2 = bottom, 3 = left
    e->vx = 0;
    e->vy = 0;
    switch (side) {
        case 0: // Top edge
            e->x = rand() % SCREEN_WIDTH;
            e->y = -20;
            e->vy = 0.4;
            break;
        case 1: // Right edge
            e->x = SCREEN_WIDTH+20;
            e->y = rand() % SCREEN_HEIGHT;
            e->vx = -0.4;
            break;
        case 2: // Bottom edge
            e->x = rand() % SCREEN_WIDTH;
            e->y = SCREEN_HEIGHT + 20;
            e->vy = -0.4;
            break;
        case 3: // Left edge
            e->x = -20;
            e->y = rand() % SCREEN_HEIGHT;
            e->vx = 0.4;
            break;
    } 
    e->angle = 0.0f;
    e->speed = 0.2f + ((float)(rand() % 100) / 100.0f);
    e->last_shot_time = SDL_GetTicks();
    e->alive = 1;
    e->jitter_angle = (float)(rand() % 628) / 100.0f;
    if (((float)(rand() % 100) / 100.0f) < 0.8) {
        e->type = ENEMY_SPIRAL;
    }
    else
        e->type = ENEMY_BASIC;

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


