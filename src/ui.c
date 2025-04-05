#include "ui.h"

void draw_ui_box(SDL_Renderer* renderer, TTF_Font* font, const PlayerShip* player) {
    char buffer1[64];
    char buffer2[64];

    sprintf(buffer1, "Material Collected: %d", player->material_collected);
    sprintf(buffer2, "Pirates Eliminated: %d", player->pirates_eliminated);

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface1 = TTF_RenderText_Solid(font, buffer1, color);
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, buffer2, color);

    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);

    int padding = 8;
    int spacing = 4;
    int total_height = surface1->h + surface2->h + spacing + padding * 2;
    int total_width = (surface1->w > surface2->w ? surface1->w : surface2->w) + padding * 2;

    SDL_Rect bg_rect = { 10, 10, total_width, total_height };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_RenderFillRect(renderer, &bg_rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bg_rect);

    SDL_Rect text1 = { bg_rect.x + padding, bg_rect.y + padding, surface1->w, surface1->h };
    SDL_Rect text2 = { bg_rect.x + padding, text1.y + surface1->h + spacing, surface2->w, surface2->h };

    SDL_RenderCopy(renderer, texture1, NULL, &text1);
    SDL_RenderCopy(renderer, texture2, NULL, &text2);

    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
}

