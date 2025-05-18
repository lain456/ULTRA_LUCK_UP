#ifndef INPUT_H
#define INPUT_H
#include "../game/game.h"



Input_box *create_input_box(Game *game, int x, int y, int w, int h, char *initial_text, SDL_Color text_color, int max_length);






void update_input_box(Game *game, Input_box *input_box);
void update_input_boxes(Game *game, Input_box *input_boxes, int size);
void render_input_box(Game *game, Input_box *input_box);
void render_input_boxes(Game *game, Input_box *input_boxes, int i_cnt);
void free_input_box(Input_box *input_box);
int input_box_is_clicked(Game *game, Input_box *input_box);
void i_pos_update(Input_box *input_box, int new_x, int new_y);
void x_order_input_boxes(Input_box *input_boxes, int margin, int i_ct);
void y_order_input_boxes(Input_box *input_boxes, int margin, int i_ct);

#endif //INPUT_H