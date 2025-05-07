//
// Created by lain on 4/12/25.
//

#ifndef SLIDER_H
#define SLIDER_H

#include "../game/game.h"
#include "../tools/tools.h"
#include "../button/button.h"











Slider* create_slider(Game* game, SDL_Rect rect);
void update_slider(Game *game,Slider* slider,int value);
void render_slider(Game* game, Slider* slider);


void handle_slider_input(Game* game, Slider* slider);
void free_slider(Slider* slider);


#endif //SLIDER_H
