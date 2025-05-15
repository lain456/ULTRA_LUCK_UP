//
// Created by lain on 5/15/25.
//

#ifndef INPUT_H
#define INPUT_H
#include "../game/game.h"

Input_box *create_input_box(Game *game, int x, int y, int w, int h, char *initial_text, SDL_Color text_color, int max_length);
void update_input_box(Game *game, Input_box *input_box);
void render_input_box(Game *game, Input_box *input_box);

#endif //INPUT_H
