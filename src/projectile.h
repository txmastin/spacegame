#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL2/SDL.h>
#include "entity.h"

#define MAX_PROJECTILES 1028

void spawn_projectile(Projectile projectiles[], float x, float y, float angle, int owner);
void update_projectiles(Projectile projectiles[], int count);
void clear_projectiles(Projectile projectiles[], int count);

#endif

