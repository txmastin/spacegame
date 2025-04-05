#pragma once
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "entity.h"

void init_space_mode(PlayerShip* player);
void update_space_mode(PlayerShip* player, SDL_Renderer* renderer, const Uint8* keystate, TTF_Font* font);

