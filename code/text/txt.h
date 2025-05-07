//
// Created by lain on 4/7/25.
//

#ifndef TXT_H
#define TXT_H
#include "txt.h"
#include "../game/game.h"



void update_txt(Text * txt,char *new_txt,SDL_Color new_color,TTF_Font *new_font);



void render_txt(Game *game,Text *txt);

Text *create_txt(char *txt,TTF_Font *new_font,SDL_Color color,int x_pos,int y_pos);


void render_textlist(Game *game,Text *txtlist,int txt_ct);



void x_center_txt(Game *game,Text *txt);

void y_center_txt(Game *game,Text *txt);



#endif //TXT_H
