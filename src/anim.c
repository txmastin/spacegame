#include <math.h>
#include "anim.h"
#include "projectile.h"

void move_player_ship(PlayerShip* player, const Uint8* keystate) {
    float thrust = player->thrust;
    float damping = 1.0f; // no damping unless brake is held

    if (keystate[SDL_SCANCODE_SPACE]) {
        thrust *= 1.5f;
    }

    if (keystate[SDL_SCANCODE_LSHIFT]) {
        damping = player->damping;
    }

    player->ax = player->ay = 0.0f;

    if (keystate[SDL_SCANCODE_W]) player->ay = -thrust;
    if (keystate[SDL_SCANCODE_S]) player->ay =  thrust;
    if (keystate[SDL_SCANCODE_A]) player->ax = -thrust;
    if (keystate[SDL_SCANCODE_D]) player->ax =  thrust;

    player->vx += player->ax;
    player->vy += player->ay;

    player->vx *= damping;
    player->vy *= damping;

    player->x += player->vx;
    player->y += player->vy;

    // Clamp to screen edges
    if (player->x < 0) {
        player->x = 0;
        player->vx = 0;
    }
    if (player->x + player->width > SCREEN_WIDTH) {
        player->x = SCREEN_WIDTH - player->width;
        player->vx = 0;
    }
    if (player->y < 0) {
        player->y = 0;
        player->vy = 0;
    }
    if (player->y + player->height > SCREEN_HEIGHT) {
        player->y = SCREEN_HEIGHT - player->height;
        player->vy = 0;
    }
}

void update_asteroids(Asteroid asteroids[], int count) {
    for (int i = 0; i < count; i++) {
        if (!asteroids[i].alive) continue;
        asteroids[i].x += asteroids[i].vx;
        asteroids[i].y += asteroids[i].vy;
    }
}

void update_enemy(Enemy* enemy, const PlayerShip* player, Projectile projectiles[], int max_projectiles) {
    if (!enemy->alive) return;

    // Distance from enemy to center of player
    float px = player->x + player->width / 2.0f;
    float py = player->y + player->height / 2.0f;
    float dx = px - enemy->x;
    float dy = py - enemy->y;
    float dist = sqrtf(dx * dx + dy * dy);

    float activation_radius = 5000.0f;
    float preferred_distance = 0.0f;
    float inertia = 0.8f;  // inertia factor (0.0 = twitchy, 1.0 = sluggish)
    
    // Repulsion from nearby player projectiles
    float repel_x = 0.0f, repel_y = 0.0f;
    for (int i = 0; i < max_projectiles; i++) {
        if (!projectiles[i].alive || projectiles[i].owner != 0) continue;

        float dxp = enemy->x - projectiles[i].x;
        float dyp = enemy->y - projectiles[i].y;
        float dist2 = dxp * dxp + dyp * dyp;

        float repel_radius = 100.0f;
        if (dist2 < repel_radius * repel_radius && dist2 > 1.0f) {
            float force = 0.65 * (100000.0f / dist2); // stronger when closer
            repel_x += dxp * force;
            repel_y += dyp * force;
        }
    }


    // If within activation range, move toward or away from player
    if (dist < activation_radius) {
        //float target_vx = (dx / dist) * enemy->speed;
        //float target_vy = (dy / dist) * enemy->speed;
       
        // Basic seek vector
        float seek_x = (dx / dist);
        float seek_y = (dy / dist);

        // Orbit direction (perpendicular to seek)
        float orbit_x = -seek_y;
        float orbit_y = seek_x;

        // Increase jitter angle slightly each frame
        enemy->jitter_angle += 0.1f + ((rand() % 20) - 10) * 0.005f;  // add a little random variation

        // Blend seek and orbit for circular approach
        float orbit_weight = 0.9f;  // tweak this: 0 = direct, 1 = pure orbit
        float jitter_sin = sinf(enemy->jitter_angle);

        float target_vx = (1.0f - orbit_weight) * seek_x + orbit_weight * orbit_x * jitter_sin;
        float target_vy = (1.0f - orbit_weight) * seek_y + orbit_weight * orbit_y * jitter_sin;

        target_vx *= enemy->speed;
        target_vy *= enemy->speed;
            
        if (dist > preferred_distance + 25) {
            // Move toward player
            target_vx = (dx / dist) * enemy->speed;
            target_vy = (dy / dist) * enemy->speed;
        } else if (dist < preferred_distance - 25) {
            // Move away from player
            target_vx = -(dx / dist) * enemy->speed;
            target_vy = -(dy / dist) * enemy->speed;
        }
        // Smooth velocity blending
        enemy->vx = inertia * enemy->vx + (1.0f - inertia) * target_vx;
        enemy->vy = inertia * enemy->vy + (1.0f - inertia) * target_vy;
        // Apply repulsion influence (optional scale)
        enemy->vx += repel_x * 0.0001f;
        enemy->vy += repel_y * 0.0001f;

    } else {
        // Outside activation radius = slow down
        enemy->vx *= 0.95f;
        enemy->vy *= 0.95f;
    }

    // Apply motion
    enemy->x += enemy->vx;
    enemy->y += enemy->vy;

    // Determine angle of current movement for aiming
    float aim_dx = enemy->vx;
    float aim_dy = enemy->vy;
    float aim_mag = sqrtf(aim_dx * aim_dx + aim_dy * aim_dy);

    Uint32 now = SDL_GetTicks();
    if (aim_mag > 0.1f && (now - enemy->last_shot_time) >= 600) {
        float angle = atan2f(aim_dy, aim_dx);  // use movement vector
        spawn_projectile(projectiles, enemy->x, enemy->y, angle, 1);
        enemy->last_shot_time = now;
    }
}


void update_enemies(Enemy enemies[], int ecount, const PlayerShip* player, Projectile projectiles[], int max_projectiles) {
    for (int i = 0; i < ecount; i++) {
        if (!enemies[i].alive) continue;
        update_enemy(&enemies[i], player, projectiles, max_projectiles);
    }
}

