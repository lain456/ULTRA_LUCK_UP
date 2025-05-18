#ifndef SLIDER_H
#define SLIDER_H
#include "../game/game.h"

Slider *create_slider(Game *game, SDL_Rect rect);
void update_slider(Game *game, Slider *s, int val);
void update_sliders(Game *game, Slider *sliders, int s_ct);
void render_slider(Game *game, Slider *s);
void render_sliders(Game *game, Slider *sliders, int s_ct);
void free_slider(Slider *s);

#endif //SLIDER_H