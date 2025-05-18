#ifndef TOOLS_H
#define TOOLS_H
#include "../game/game.h"

SDL_Surface *create_color_surface(int w, int h, Uint8 r, Uint8 g, Uint8 b);
SDL_Surface *scaleSurface(SDL_Surface *surface, int width, int height);
SDL_Surface *create_blank_surface(int w, int h);
Text *safe_create_txt(const char *text, TTF_Font *font, SDL_Color color, int x_pos, int y_pos);
void toggle_fullscreen(Game *game);
void draw_rect(SDL_Surface *surface, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);

#endif //TOOLS_H