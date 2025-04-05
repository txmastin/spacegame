#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "entity.h"

void draw_ui_box(SDL_Renderer* renderer, TTF_Font* font, const PlayerShip* player);

#endif // UI_H
