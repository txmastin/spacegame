#include "ui.h"
#include "space_mode.h"


void draw_ui_box(SDL_Renderer* renderer, TTF_Font* font, const PlayerShip* player) {
    char buffer1[64];
    char buffer2[64];
    char buffer3[64];

    sprintf(buffer1, "Material Collected: %d", player->material_collected);
    sprintf(buffer2, "Pirates Eliminated: %d", player->pirates_eliminated);

    Uint32 ms_remaining = get_space_mode_time_remaining();
    int seconds = ms_remaining / 1000;
    int minutes = seconds / 60;
    seconds = seconds % 60;
    sprintf(buffer3, "Time Remaining: %d:%02d", minutes, seconds);

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface1 = TTF_RenderText_Solid(font, buffer1, color);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, buffer2, color);
    SDL_Surface* surface3 = TTF_RenderText_Solid(font, buffer3, color);
    
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    
    int padding = 8;
    int spacing = 4;

    int max_width = surface1->w;
    if (surface2->w > max_width) max_width = surface2->w;
    if (surface3->w > max_width) max_width = surface3->w;

    int total_height = surface1->h + surface2->h + surface3->h + spacing * 2 + padding * 2;
    int total_width = max_width + padding * 2;

    SDL_Rect bg_rect = { 10, 10, total_width, total_height };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &bg_rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bg_rect);

    SDL_Rect text1 = { bg_rect.x + padding, bg_rect.y + padding, surface1->w, surface1->h };
    SDL_Rect text2 = { bg_rect.x + padding, text1.y + surface1->h + spacing, surface2->w, surface2->h };
    SDL_Rect text3 = { bg_rect.x + padding, text2.y + surface2->h + spacing, surface3->w, surface3->h };

    SDL_RenderCopy(renderer, texture1, NULL, &text1);
    SDL_RenderCopy(renderer, texture2, NULL, &text2);
    SDL_RenderCopy(renderer, texture3, NULL, &text3);
    
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
}

