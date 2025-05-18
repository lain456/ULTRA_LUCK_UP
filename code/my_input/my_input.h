#ifndef MY_INPUT_H
#define MY_INPUT_H
#include <game/game.h>

typedef struct {
    SDL_Color not_hovered_color;
    SDL_Color hovered_color;
    SDL_Color active_color;
} InputStyle;

void init_my_input(Game *game, My_input *my_input, int x, int y, int w, int h, InputStyle *style);
void handle_my_input_event(Game *game, My_input *my_input, SDL_Event *event);
void render_my_input(Game *game, My_input *my_input);
void render_my_inputs(Game *game, My_input *my_input, int i_ct);

#endif //MY_INPUT_H