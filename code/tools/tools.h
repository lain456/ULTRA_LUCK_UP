//
// Created by lain on 4/3/25.
//

#ifndef TOOLS_H
#define TOOLS_H
#include "../game/game.h"


// functions :
SDL_Surface *create_color_surface(int w , int h , Uint8 r, Uint8 g, Uint8 b );


SDL_Surface* scaleSurface(SDL_Surface* surface, int width, int height);


SDL_Surface *create_blank_surface(int w, int h);



SDL_Surface *create_centered_label(SDL_Rect targetRect, TTF_Font *font, const char *text, SDL_Color color);


void toggle_fullscreen(Game *game) ;


#endif //TOOLS_H

