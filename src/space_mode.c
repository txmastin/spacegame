#include "space_mode.h"
#include "anim.h"
#include "render.h"
#include "spawn.h"
#include "projectile.h"
#include "collision.h"
#include "ui.h"

static Asteroid asteroids[ASTEROID_COUNT];
static Enemy enemies[ENEMY_COUNT];
static Projectile projectiles[MAX_PROJECTILES] = {0};
static Star stars[STAR_COUNT];

static Uint32 last_enemy_spawn_time = 0;

static Uint32 space_mode_start_time = 0;
static const Uint32 SPACE_MODE_DURATION_MS = 5 * 60 * 1000;

void start_space_mode() {
    space_mode_start_time = SDL_GetTicks();
}

Uint32 get_space_mode_time_remaining() {
    Uint32 now = SDL_GetTicks();
    Uint32 elapsed = now - space_mode_start_time;
    return (elapsed >= SPACE_MODE_DURATION_MS) ? 0 : (SPACE_MODE_DURATION_MS - elapsed);
}


void init_space_mode(PlayerShip* player) {
    *player = (PlayerShip){
        .width = 25, .height = 15,
        .x = (SCREEN_WIDTH - 15) / 2.0f,
        .y = (SCREEN_HEIGHT - 15) / 2.0f,
        .vx = 0, .vy = 0, .ax = 0, .ay = 0,
        .thrust = 0.1f,
        .damping = 0.95f,
        .angle = 0.0f,
        .material_collected = 0,
        .pirates_eliminated = 0
    };

    spawn_asteroids(asteroids, ASTEROID_COUNT);
    spawn_stars(stars, STAR_COUNT);
}

void update_space_mode(PlayerShip* player, SDL_Renderer* renderer, const Uint8* keystate, TTF_Font* font) {
    int mouse_x, mouse_y;
    Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

    // Face mouse
    float cx = player->x + player->width / 2.0f;
    float cy = player->y + player->height / 2.0f;
    player->angle = atan2f(mouse_y - cy, mouse_x - cx);

    // Shoot
    static int last_shot = 0;
    Uint32 now = SDL_GetTicks();
    if ((mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) && now - last_shot > 300) {
        float muzzle_offset = player->width;
        float muzzle_x = cx + cosf(player->angle) * muzzle_offset;
        float muzzle_y = cy + sinf(player->angle) * muzzle_offset;
        spawn_projectile(projectiles, muzzle_x, muzzle_y, player->angle, 0);
        last_shot = now;
    }

    // Enemy spawn logic
    int enemies_alive = 0;
    for (int i = 0; i < ENEMY_COUNT; i++)
        if (enemies[i].alive) enemies_alive++;

    if (now - last_enemy_spawn_time > 10000 && enemies_alive < ENEMY_COUNT) {
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (!enemies[i].alive) {
                spawn_enemy(&enemies[i]);
                last_enemy_spawn_time = now;
                break;
            }
        }
    }
    
    // Asteroid spawn logic
    for (int i = 0; i < ASTEROID_COUNT; i++) {
        if (!asteroids[i].alive) {
            spawn_asteroid(&asteroids[i]);
            break;
        }
    }



    move_player_ship(player, keystate);
    update_enemies(enemies, ENEMY_COUNT, player, projectiles, MAX_PROJECTILES);
    update_projectiles(projectiles, MAX_PROJECTILES);
    handle_collisions(player, projectiles, MAX_PROJECTILES, asteroids, ASTEROID_COUNT, enemies, ENEMY_COUNT, now);
    update_asteroids(asteroids, ASTEROID_COUNT);

    draw_stars(renderer, stars, STAR_COUNT);
    draw_player_ship(renderer, player);
    draw_asteroids(renderer, asteroids, ASTEROID_COUNT);
    draw_enemies(renderer, enemies, ENEMY_COUNT);
    draw_projectiles(renderer, projectiles, MAX_PROJECTILES);
    draw_mining_beam(renderer, player, asteroids, ASTEROID_COUNT);
    draw_ui_box(renderer, font, player);
}

