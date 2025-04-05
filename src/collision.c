#include "collision.h"
#include <math.h>
#include <stdio.h>

static int check_collision_circle_rect(float cx, float cy, float radius, SDL_Rect rect) {
    float closestX = fmaxf(rect.x, fminf(cx, rect.x + rect.w));
    float closestY = fmaxf(rect.y, fminf(cy, rect.y + rect.h));
    float dx = cx - closestX;
    float dy = cy - closestY;
    return (dx * dx + dy * dy) < (radius * radius);
}

static int check_circle_point(float cx, float cy, float radius, float px, float py) {
    float dx = cx - px;
    float dy = cy - py;
    return (dx * dx + dy * dy) <= radius * radius;
}

void handle_collisions(
    PlayerShip* player,
    Projectile projectiles[], int pcount,
    Asteroid asteroids[], int acount,
    Enemy enemies[], int ecount,
    Uint32 current_time
) {
    SDL_Rect player_rect = {
        (int)player->x,
        (int)player->y,
        player->width,
        player->height
    };

    // Projectile logic
    for (int i = 0; i < pcount; i++) {
        if (!projectiles[i].alive) continue;

        // Player hit by projectile
        if (projectiles[i].owner == 1 && check_collision_circle_rect(projectiles[i].x, projectiles[i].y, 2.0f, player_rect)) {
            SDL_Delay(1000);

            player->x = (1920 - player->width) / 2.0f;
            player->y = (1080 - player->height) / 2.0f;
            player->vx = player->vy = player->ax = player->ay = 0;
            player->material_collected = 0;
            player->pirates_eliminated =0;
            for (int j = 0; j < pcount; j++) {
                projectiles[j].alive = 0;
            }
            for (int j = 0; j < ecount; j++) {
                enemies[j].alive = 0;
            }
            
            spawn_asteroids(asteroids, acount);
            return;
        }

        // Projectiles hit asteroids (but don't damage them)
        for (int j = 0; j < acount; j++) {
            if (!asteroids[j].alive) continue;

            if (check_circle_point(
                asteroids[j].x, asteroids[j].y, asteroids[j].radius,
                projectiles[i].x, projectiles[i].y)) {
                projectiles[i].alive = 0;
                break;
            }
        }
    
        // Projectile hits enemy
        
        if (projectiles[i].alive && projectiles[i].owner == 0) {
            for (int j = 0; j < ecount; j++) {
                if (!enemies[j].alive) continue;

                float dx = enemies[j].x - projectiles[i].x;
                float dy = enemies[j].y - projectiles[i].y;
                float dist2 = dx * dx + dy * dy;

                float hit_radius = 16.0f;  // slightly larger than enemy sprite

                if (dist2 < hit_radius * hit_radius) {
                    enemies[j].alive = 0;
                    projectiles[i].alive = 0;
                    player->pirates_eliminated += 1;
                    break;
                }
            }
        }
    }

    // Mining beam logic
    float px = player->x + player->width / 2.0f;
    float py = player->y + player->height / 2.0f;

    static Uint32 last_mine_time = 0;
    if (current_time - last_mine_time >= 1000) {
        last_mine_time = current_time;
        for (int i = 0; i < acount; i++) {
            if (!asteroids[i].alive) continue;

            asteroids[i].being_mined = 0;
            float dx = asteroids[i].x - px;
            float dy = asteroids[i].y - py;
            float dist2 = dx * dx + dy * dy;

            if (dist2 < 100 * 100) {
                asteroids[i].being_mined = 1;
                asteroids[i].hits_taken++;
                if (asteroids[i].hits_taken >= asteroids[i].hits_required) {
                    asteroids[i].alive = 0;
                    player->material_collected += (int)(asteroids[i].radius * 5.0f);
                }
            }
        }
    }
   
    // Asteroid logic
    for (int i = 0; i < acount; i++) {
        if (!asteroids[i].alive) continue;

        // Player-asteroid elastic collision
        float dx = (player->x + player->width / 2.0f) - asteroids[i].x;
        float dy = (player->y + player->height / 2.0f) - asteroids[i].y;
        float dist_sq = dx * dx + dy * dy;
        float min_dist = asteroids[i].radius + player->width / 2.0f;

        if (dist_sq < min_dist * min_dist) {
            float dist = sqrtf(dist_sq);
            if (dist == 0) dist = 0.01f; // avoid division by zero

            float nx = dx / dist;
            float ny = dy / dist;

            float relative_vx = player->vx - asteroids[i].vx;
            float relative_vy = player->vy - asteroids[i].vy;
            float dot = relative_vx * nx + relative_vy * ny;

            if (dot < 0) {
                float impulse = 1.0f * dot;  // rescalable to tune feeling

                player->vx -= impulse * nx;
                player->vy -= impulse * ny;

                // Asteroid gains a small amount of motion
                asteroids[i].vx += 0.1f * impulse * nx;
                asteroids[i].vy += 0.1f * impulse * ny;
            }
        }
         
        // Enemy-asteroid elastic collision
        for(int j = 0; j < ecount; j++) {
            if (!enemies[j].alive) continue;

            float dx = (enemies[j].x + 18 / 2.0f) - asteroids[i].x;
            float dy = (enemies[j].y + 18 / 2.0f) - asteroids[i].y;
            float dist_sq = dx * dx + dy * dy;
            
            float min_dist = asteroids[i].radius + 24 / 2.0f;

            if (dist_sq < min_dist * min_dist) {
                float dist = sqrtf(dist_sq);
                if (dist == 0) dist = 0.01f; // avoid division by zero

                float nx = dx / dist;
                float ny = dy / dist;

                float relative_vx = enemies[j].vx - asteroids[i].vx;
                float relative_vy = enemies[j].vy - asteroids[i].vy;
                float dot = relative_vx * nx + relative_vy * ny;

                if (dot < 0) {
                    float impulse = 1.0f * dot;  

                    enemies[j].vx -= impulse * nx;
                    enemies[j].vy -= impulse * ny;

                    // Asteroid gains a small amount of motion
                    asteroids[i].vx += 0.1f * impulse * nx;
                    asteroids[i].vy += 0.1f * impulse * ny;
                }
            }
        }

        // collision between asteroids 
        for (int j = i + 1; j < acount; j++) {
            if (!asteroids[j].alive) continue;

            float dx = asteroids[j].x - asteroids[i].x;
            float dy = asteroids[j].y - asteroids[i].y;
            float dist2 = dx * dx + dy * dy;
            float rsum = asteroids[i].radius + asteroids[j].radius;

            if (dist2 < rsum * rsum && dist2 > 0.01f) {
                float dist = sqrtf(dist2);
                float nx = dx / dist;
                float ny = dy / dist;

                // Simple elastic velocity exchange along normal
                float dvx = asteroids[j].vx - asteroids[i].vx;
                float dvy = asteroids[j].vy - asteroids[i].vy;
                float dot = dvx * nx + dvy * ny;

                if (dot < 0) {
                    // --- Mass-based elastic impulse ---
                    float mass_i = asteroids[i].radius;
                    float mass_j = asteroids[j].radius;
                    float total_mass = mass_i + mass_j;

                    // Classic elastic collision response
                    float impulse = (-0.01f * dot) / total_mass;

                    asteroids[i].vx -= impulse * mass_j * nx;
                    asteroids[i].vy -= impulse * mass_j * ny;
                    asteroids[j].vx += impulse * mass_i * nx;
                    asteroids[j].vy += impulse * mass_i * ny;
                }
                if (rsum - dist > 1.0f) {
                    float overlap = 0.1f * (rsum - dist);  // even smaller push
                    asteroids[i].x -= nx * overlap;
                    asteroids[i].y -= ny * overlap;
                    asteroids[j].x += nx * overlap;
                    asteroids[j].y += ny * overlap;
                }
            }
        }
        
        // check if asteroids hit edge of screen
        if (asteroids[i].x - asteroids[i].radius < 0) {
            asteroids[i].x = asteroids[i].radius;
            asteroids[i].vx *= -1;
        } else if (asteroids[i].x + asteroids[i].radius > 1920) {
            asteroids[i].x = 1920 - asteroids[i].radius;
            asteroids[i].vx *= -1;
        }

        if (asteroids[i].y - asteroids[i].radius < 0) {
            asteroids[i].y = asteroids[i].radius;
            asteroids[i].vy *= -1;
        } else if (asteroids[i].y + asteroids[i].radius > 1080) {
            asteroids[i].y = 1080 - asteroids[i].radius;
            asteroids[i].vy *= -1;
        }
    }
}

