#ifndef MY_INPUT_H
#define MY_INPUT_H
#include <game/game.h>



void init_my_input(Game *game, My_input *my_input, int x, int y, int w, int h, InputStyle *style);
void handle_my_input_event(Game *game, My_input *my_input, SDL_Event *event);
void render_my_input(Game *game, My_input *my_input);
void render_my_inputs(Game *game, My_input *my_input, int i_ct);
void update_input_text(My_input *my_input);
 void sync_input(Game *game, My_input *my_input, int input_index);
void reset_controls(Game *game, My_input *my_inputlist);

#endif //MY_INPUT_H