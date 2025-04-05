#include "station_mode.h"
#include <SDL.h>
#include <stdio.h>

static SDL_FRect player = { 100, 100, 16, 16 };

void update_station_mode(const Uint8* keystate) {
    float speed = 2.0f;
    if (keystate[SDL_SCANCODE_W]) player.y -= speed;
    if (keystate[SDL_SCANCODE_S]) player.y += speed;
    if (keystate[SDL_SCANCODE_A]) player.x -= speed;
    if (keystate[SDL_SCANCODE_D]) player.x += speed;

    // Later: check for NPC or hangar collision here
}

void draw_station_mode(SDL_Renderer* renderer) {
    // Background
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Player
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer, &player);

    // Temp NPCs
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    SDL_FRect npc1 = { 300, 100, 16, 16 };
    SDL_RenderFillRectF(renderer, &npc1);

    // Hangar door
    SDL_SetRenderDrawColor(renderer, 150, 255, 150, 255);
    SDL_FRect hangar = { 600, 400, 32, 32 };
    SDL_RenderFillRectF(renderer, &hangar);
}

