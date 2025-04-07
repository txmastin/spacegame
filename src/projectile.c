#include "projectile.h"
#include <math.h>

void spawn_projectile(Projectile projectiles[], float x, float y, float angle, int owner) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].alive) {
            projectiles[i].x = x;
            projectiles[i].y = y;
            if (owner == 0) { 
                projectiles[i].vx = cosf(angle) * 3.0f;
                projectiles[i].vy = sinf(angle) * 3.0f;
            }
            else {
                projectiles[i].vx = cosf(angle) * 1.8f;
                projectiles[i].vy = sinf(angle) * 1.8f;
            }
            projectiles[i].angle = angle;
            projectiles[i].alive = 1;
            projectiles[i].owner = owner;
            break;
        }
    }
}


void update_projectiles(Projectile projectiles[], int count) {
    for (int i = 0; i < count; i++) {
        if (!projectiles[i].alive) continue;

        projectiles[i].x += projectiles[i].vx;
        projectiles[i].y += projectiles[i].vy;

        // Kill offscreen projectiles
        if (projectiles[i].x < -50 || projectiles[i].x > 1920 + 50 ||
            projectiles[i].y < -50 || projectiles[i].y > 1080 + 50) {
            projectiles[i].alive = 0;
        }

        // Kill projectiles with zero velocity (optional but helpful)
        if (projectiles[i].vx == 0.0f && projectiles[i].vy == 0.0f) {
            printf("Projectile %d has zero velocity — killing it\n", i);
            projectiles[i].alive = 0;
        }

        // Optional debug
        if (isnan(projectiles[i].x) || isnan(projectiles[i].y)) {
            printf("Projectile %d has NaN coords! (x=%.2f, y=%.2f)\n", i, projectiles[i].x, projectiles[i].y);
            projectiles[i].alive = 0;
        }
    }
}



void clear_projectiles(Projectile projectiles[], int count) {
    for (int i = 0; i < count; i++) {
        projectiles[i].alive = 0;
    }
}

