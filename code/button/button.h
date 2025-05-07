//
// Created by lain on 4/3/25.
//

#ifndef BUTTON_H
#define BUTTON_H
#include "../game/game.h"






Button *create_buttons(Game *game ,char *text_list[],int b_x,int b_y,SDL_Color color,int margin ,int type);

void render_buttons(Game *game, Button button[],int b_cnt ) ;



//Button create_button( char *text_list[],i;


void render_button(Game *game,Button *b,int hover);



Button *create_button(Game *game ,int x,int y,int h,int w ,char* text ,SDL_Color color ,int type );


int is_hovered(Game *game, SDL_Rect *rect);

void update_buttons(Game *game, Button *buttons,int size);





int is_pressed(Game *game, SDL_Rect *rect);


int is_clicked(Game *game, SDL_Rect *rect);

int button_is_clicked(Game *game, Button *button);



void x_order_buttons(Button *button,int margin , int b_ct);

void y_order_buttons(Button *button,int margin , int b_ct);

void b_pos_update(Button *button,int new_x,int new_y);









#endif //BUTTON_H
