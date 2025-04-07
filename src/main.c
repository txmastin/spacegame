#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include "game_mode.h"
#include "station_mode.h"
#include "space_mode.h"
#include "entity.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Space Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    TTF_Font* font = TTF_OpenFont("assets/fonts/VCR_OSD_MONO_1.001.ttf", 24);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    GameMode current_mode = MODE_STATION;
    int running = 1;

    // Mode-specific state
    PlayerShip player;
    init_space_mode(&player);  // initializes asteroids, enemies, player, etc.

    Uint32 last_time = SDL_GetTicks();
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    Uint8 prev_keystate[SDL_NUM_SCANCODES] = {0};

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        SDL_PumpEvents();  // updates key state before checking keys

        keystate = SDL_GetKeyboardState(NULL); // get updated key state
        
        if (keystate[SDL_SCANCODE_RETURN] && !prev_keystate[SDL_SCANCODE_RETURN]) {
            if (current_mode == MODE_STATION) {
                start_space_mode();  // new function
                current_mode = MODE_SPACE;
            } else {
                current_mode = MODE_STATION;
            }
        }
        
        SDL_memcpy(prev_keystate, keystate, SDL_NUM_SCANCODES); // store old
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (current_mode == MODE_STATION) {
            update_station_mode(keystate);
            draw_station_mode(renderer);
        } else if (current_mode == MODE_SPACE) {
            if (get_space_mode_time_remaining() == 0)
                current_mode = MODE_STATION;
            else
            update_space_mode(&player, renderer, keystate, font);
        }

        SDL_RenderPresent(renderer);

        Uint32 frame_time = SDL_GetTicks() - last_time;
        if (frame_time < 16) SDL_Delay(16 - frame_time);
        last_time = SDL_GetTicks();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

