#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>
#include "entity.h"
#include "projectile.h"
#include "spawn.h"

void handle_collisions(
    PlayerShip* player,
    Projectile projectiles[], int pcount,
    Asteroid asteroids[], int acount,
    Enemy enemies[], int ecount,
    Uint32 current_time
);

#endif

