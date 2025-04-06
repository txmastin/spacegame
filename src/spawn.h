#ifndef SPAWN_H
#define SPAWN_H

#include "entity.h"

#define ASTEROID_COUNT 15
#define ENEMY_COUNT 7
#define STAR_COUNT 500

void spawn_asteroids(Asteroid asteroids[], int count);
void spawn_asteroid(Asteroid* asteroid);
void spawn_enemy(Enemy* enemy);
void spawn_enemies(Enemy enemies[], int count);
void spawn_stars(Star stars[], int count);
#endif

