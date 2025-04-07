#ifndef SPAWN_H
#define SPAWN_H

#include "entity.h"

#define ASTEROID_COUNT 25
#define ENEMY_COUNT 14
#define STAR_COUNT 1000
#define STREAK_STAR_COUNT 150

void generate_asteroid_outline(Asteroid* asteroid);
void spawn_asteroids(Asteroid asteroids[], int count);
void spawn_asteroid(Asteroid* asteroid);
void spawn_enemy(Enemy* enemy);
void spawn_enemies(Enemy enemies[], int count);
void spawn_stars(Star stars[], int count);
void spawn_streak_stars(StreakStar stars[], int count);

#endif

