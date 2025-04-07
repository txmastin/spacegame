#ifndef ANIM_H
#define ANIM_H

#include <SDL2/SDL.h>
#include "entity.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

void move_player_ship(PlayerShip* player, const Uint8* keystate);
void update_asteroids(Asteroid asteroids[], int count);
void update_streak_stars(StreakStar stars[], int count);
void update_enemy(Enemy* enemy, const PlayerShip* player, Projectile projectiles[], int max_projectiles);
void update_enemies(Enemy enemies[], int ecount, const PlayerShip* player, Projectile projectiles[], int max_projectiles);

#endif

