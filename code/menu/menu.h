
//
// Created by lain on 4/8/25.
//

#ifndef MENU_H
#define MENU_H


#include "../game/game.h"
#include"../text/txt.h"
#include "../button/button.h"
#include"../Background/background.h"







void Init_Menu(Menu *menu);
void render_menu(Game *game, Menu *menu);
void node_Init(M_node *node, Menu *menu, int id);
void free_menu(Menu *menu);



#endif //MENU_H




