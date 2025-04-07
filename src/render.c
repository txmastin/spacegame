#include "render.h"
#include <math.h>

#define M_PI 3.1415926535

void draw_stars(SDL_Renderer* renderer, const Star stars[], int count) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White

    for (int i = 0; i < count; i++) {
        SDL_RenderDrawPointF(renderer, stars[i].x, stars[i].y);
    }
}

void draw_streak_stars(SDL_Renderer* renderer, const StreakStar stars[], int count) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
    for (int i = 0; i < count; i++) {
        SDL_RenderDrawLine(renderer,
            (int)stars[i].x, (int)stars[i].y,
            (int)(stars[i].x - stars[i].length), (int)stars[i].y);
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


void draw_asteroids(SDL_Renderer* renderer, Asteroid asteroids[], int acount) {
    for (int i = 0; i < acount; i++) {
        if (!asteroids[i].alive) continue;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Draw jagged outline
        for (int j = 0; j < asteroids[i].vertex_count; j++) {
            int next = (j + 1) % asteroids[i].vertex_count;
            SDL_RenderDrawLineF(
                renderer,
                asteroids[i].x + asteroids[i].outline[j].x,
                asteroids[i].y + asteroids[i].outline[j].y,
                asteroids[i].x + asteroids[i].outline[next].x,
                asteroids[i].y + asteroids[i].outline[next].y
            );
        }

        // Draw health bar if mining beam is active
        if (asteroids[i].being_mined) {
            float health_ratio = 1.0f - (float)asteroids[i].hits_taken / (float)asteroids[i].hits_required;
            if (health_ratio < 0.0f) health_ratio = 0.0f;

            float bar_width = asteroids[i].radius * 2;
            float bar_height = 4;

            SDL_FRect bar_bg = {
                asteroids[i].x - bar_width / 2.0f,
                asteroids[i].y - asteroids[i].radius - 10,
                bar_width,
                bar_height
            };

            SDL_FRect bar_fg = bar_bg;
            bar_fg.w *= health_ratio;

            // Draw background (gray)
            SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
            SDL_RenderFillRectF(renderer, &bar_bg);

            // Draw foreground (white)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRectF(renderer, &bar_fg);
        }
    }
}


/****** old no health bar *****
void draw_asteroids(SDL_Renderer* renderer, const Asteroid asteroids[], int count) {
    SDL_SetRenderDrawColor(renderer, 110, 102, 95, 255);
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
*/


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
            x - 2.5,  // center horizontally
            y - 2.5,  // center vertically
            5, 5
        };
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void draw_enemy(SDL_Renderer* renderer, const Enemy* enemy) {
    if (!enemy->alive) return;

    if (enemy->type == ENEMY_SPIRAL) {
        SDL_SetRenderDrawColor(renderer, 50, 100, 255, 255);

        const int sides = 8;
        const float radius = 24.0f;
        float cx = enemy->x;
        float cy = enemy->y;

        for (int i = 0; i < sides; i++) {
            float a1 = 2 * M_PI * i / sides;
            float a2 = 2 * M_PI * (i + 1) / sides;
            SDL_RenderDrawLineF(
                renderer,
                cx + cosf(a1) * radius, cy + sinf(a1) * radius,
                cx + cosf(a2) * radius, cy + sinf(a2) * radius
            );
        }
    }
    else {


        float x = enemy->x;
        float y = enemy->y;
        float angle = enemy->angle;

        float size = 10.0f;

        // Calculate triangle points
        float tip_x = x + cosf(angle) * size;
        float tip_y = y + sinf(angle) * size;

        float left_x = x + cosf(angle + 2.5f) * size;
        float left_y = y + sinf(angle + 2.5f) * size;

        float right_x = x + cosf(angle - 2.5f) * size;
        float right_y = y + sinf(angle - 2.5f) * size;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLineF(renderer, tip_x, tip_y, left_x, left_y);
        SDL_RenderDrawLineF(renderer, left_x, left_y, right_x, right_y);
        SDL_RenderDrawLineF(renderer, right_x, right_y, tip_x, tip_y);
    }
}

/***** Old rectangle *****
void draw_enemy(SDL_Renderer* renderer, const Enemy* enemy) {
    if (!enemy->alive) return;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { (int)enemy->x - 10, (int)enemy->y - 10, 20, 20 };
    SDL_RenderFillRect(renderer, &rect);
}
*/

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

