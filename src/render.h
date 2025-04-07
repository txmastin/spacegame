#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "entity.h"

void draw_stars(SDL_Renderer* renderer, const Star stars[], int count);
void draw_streak_stars(SDL_Renderer* renderer, const StreakStar stars[], int count);
void draw_player_ship(SDL_Renderer* renderer, const PlayerShip* player);
void draw_asteroids(SDL_Renderer* renderer, Asteroid asteroids[], int count);
void draw_projectiles(SDL_Renderer* renderer, const Projectile projectiles[], int count);
void draw_enemy(SDL_Renderer* renderer, const Enemy* enemy);
void draw_enemies(SDL_Renderer* renderer, const Enemy enemies[], int ecount);
void draw_mining_beam(SDL_Renderer* renderer, const PlayerShip* player, const Asteroid asteroids[], int count);

#endif

