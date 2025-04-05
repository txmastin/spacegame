#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#include "entity.h"
#include "anim.h"
#include "render.h"
#include "spawn.h"
#include "projectile.h"
#include "collision.h"
#include "ui.h"

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    
    if(TTF_Init() < 0) {
        fprintf(stderr,"Failed to initialize SDL_ttf: %s\n", TTF_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Space Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
 
    TTF_Font* font = TTF_OpenFont("assets/fonts/VCR_OSD_MONO_1.001.ttf", 24);
        if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    PlayerShip player = {
        .width = 25,
        .height = 15,
        .x = (SCREEN_WIDTH - 15) / 2.0f,
        .y = (SCREEN_HEIGHT - 15) / 2.0f,
        .vx = 0, .vy = 0, .ax = 0, .ay = 0,
        .thrust = 0.1f,
        .damping = 0.95f,
        .angle = 0.0f,
        .material_collected = 0,
        .pirates_eliminated =0
    };
    
    Uint32 last_enemy_spawn_time = 0;
    int enemies_alive = 0;

    Asteroid asteroids[ASTEROID_COUNT];
    Projectile projectiles[MAX_PROJECTILES] = {0};
    Enemy enemies[ENEMY_COUNT];
    Star stars[STAR_COUNT]; 

    spawn_asteroids(asteroids, ASTEROID_COUNT);
    //spawn_enemies(enemies, ENEMY_COUNT);
    spawn_stars(stars, STAR_COUNT);
    

    int running = 1;
    Uint32 last_time = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        int mouse_x, mouse_y;
        Uint32 mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        // Update angle to face mouse
        float cx = player.x + player.width / 2.0f;
        float cy = player.y + player.height / 2.0f;
        player.angle = atan2f(mouse_y - cy, mouse_x - cx);

        // Fire projectile on click
        static int last_shot = 0;
        if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            Uint32 now = SDL_GetTicks();
            if (now - last_shot > 300) {
                float muzzle_offset = player.width * 1.0;
                float muzzle_x = cx + cosf(player.angle) * muzzle_offset;
                float muzzle_y = cy + sinf(player.angle) * muzzle_offset;
                spawn_projectile(projectiles, muzzle_x, muzzle_y, player.angle, 0);
                last_shot = now;
            }
        }
        Uint32 now = SDL_GetTicks();

        // Count currently alive enemies
        enemies_alive = 0;
        for (int i = 0; i < ENEMY_COUNT; i++) {
            if (enemies[i].alive) enemies_alive++;
        }

        // If enough time has passed and we have room to spawn
        if (now - last_enemy_spawn_time > 10000 && enemies_alive < ENEMY_COUNT) {
            for (int i = 0; i < ENEMY_COUNT; i++) {
                if (!enemies[i].alive) {
                    spawn_enemy(&enemies[i]);
                    last_enemy_spawn_time = now;
                    break; // spawn only one per cycle
                }
            }
        }


        // Movement and update
        move_player_ship(&player, keystate);
        update_enemies(enemies, ENEMY_COUNT, &player, projectiles, MAX_PROJECTILES);
        update_projectiles(projectiles, MAX_PROJECTILES);

        now = SDL_GetTicks();
        handle_collisions(&player, projectiles, MAX_PROJECTILES, asteroids, ASTEROID_COUNT, enemies, ENEMY_COUNT, now);
        update_asteroids(asteroids, ASTEROID_COUNT); 


        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        draw_stars(renderer, stars, STAR_COUNT);
        draw_player_ship(renderer, &player);
        draw_asteroids(renderer, asteroids, ASTEROID_COUNT);
        draw_enemies(renderer, enemies, ENEMY_COUNT);
        draw_projectiles(renderer, projectiles, MAX_PROJECTILES);
        draw_mining_beam(renderer, &player, asteroids, ASTEROID_COUNT);

        draw_ui_box(renderer, font, &player);
        /*
        // Text rendering:
        char buffer[64];
        sprintf(buffer, "Material Collected: %d", player.material_collected);

        SDL_Color color = { 255, 255, 255, 255 };
        SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int padding = 8;
        SDL_Rect text_rect = { 0 };
        text_rect.x = 10 + padding;
        text_rect.y = 10 + padding;
        text_rect.w = surface->w;
        text_rect.h = surface->h;

        // Background box (black)
        SDL_Rect bg_rect = {
            text_rect.x - padding,
            text_rect.y - padding,
            text_rect.w + padding * 2,
            text_rect.h + padding * 2
        };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60); 
        SDL_RenderFillRect(renderer, &bg_rect);

        // Border (white)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &bg_rect);

        // Draw text
        SDL_RenderCopy(renderer, texture, NULL, &text_rect);

        // Cleanup
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        */

        SDL_RenderPresent(renderer);

        // Frame delay
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

