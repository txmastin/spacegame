#include "render.h"
#include <math.h>

void draw_stars(SDL_Renderer* renderer, const Star stars[], int count) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

    for (int i = 0; i < count; i++) {
        SDL_RenderDrawPointF(renderer, stars[i].x, stars[i].y);
    }
}

void draw_player_ship(SDL_Renderer* renderer, const PlayerShip* player) {
    float cx = player->x + player->width / 2.0f;
    float cy = player->y + player->height / 2.0f;
    float w = player->width;
    float h = player->height;
    float a = player->angle;

    SDL_FPoint corners[4] = {
        { -w/2, -h/2 }, { w/2, -h/2 }, { w/2, h/2 }, { -w/2, h/2 }
    };

    for (int i = 0; i < 4; i++) {
        float x = corners[i].x;
        float y = corners[i].y;
        corners[i].x = cosf(a) * x - sinf(a) * y + cx;
        corners[i].y = sinf(a) * x + cosf(a) * y + cy;
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < 4; i++) {
        SDL_RenderDrawLineF(renderer, corners[i].x, corners[i].y,
                            corners[(i+1)%4].x, corners[(i+1)%4].y);
    }
}

void draw_asteroids(SDL_Renderer* renderer, const Asteroid asteroids[], int count) {
    SDL_SetRenderDrawColor(renderer, 50, 150, 255, 255);
    for (int i = 0; i < count; i++) {
        if (!asteroids[i].alive) continue;
        for (int dy = -asteroids[i].radius; dy <= asteroids[i].radius; dy++) {
            int dx = (int)sqrtf(asteroids[i].radius * asteroids[i].radius - dy * dy);
            int cx = (int)asteroids[i].x;
            int cy = (int)asteroids[i].y;
            SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
        }
    }
}

void draw_projectiles(SDL_Renderer* renderer, const Projectile projectiles[], int count) {
    for (int i = 0; i < count; i++) {
        if (!projectiles[i].alive) continue;

        float x = projectiles[i].x;
        float y = projectiles[i].y;

        // Skip drawing if way offscreen
        if (x < -20 || x > 1920 + 20 || y < -20 || y > 1080 + 20) {
            continue;
        }

        // Color by owner
        switch (projectiles[i].owner) {
            case 0: // Player
                SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255);  // green
                break;
            case 1: // Enemy
                SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);  // red
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // gray fallback
                break;
        }

        SDL_FRect rect = {
            x - 3,  // center horizontally
            y - 1,  // center vertically
            6, 2
        };
        SDL_RenderFillRectF(renderer, &rect);
    }
}



void draw_enemy(SDL_Renderer* renderer, const Enemy* enemy) {
    if (!enemy->alive) return;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { (int)enemy->x - 10, (int)enemy->y - 10, 20, 20 };
    SDL_RenderFillRect(renderer, &rect);
}

void draw_mining_beam(SDL_Renderer* renderer, const PlayerShip* player, const Asteroid asteroids[], int count) {
    float px = player->x + player->width / 2.0f;
    float py = player->y + player->height / 2.0f;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < count; i++) {
        if (!asteroids[i].alive) continue;

        float dx = asteroids[i].x - px;
        float dy = asteroids[i].y - py;
        float dist2 = dx * dx + dy * dy;

        if (dist2 < 100 * 100) {  // mining radius
            SDL_RenderDrawLineF(renderer, px, py, asteroids[i].x, asteroids[i].y);
        }
    }
}

void draw_enemies(SDL_Renderer* renderer, const Enemy enemies[], int ecount) {
    for (int i = 0; i < ecount; i++) {
        if (!enemies[i].alive) continue;
        draw_enemy(renderer, &enemies[i]);
    }
}

