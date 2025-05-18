#include "menu.h"
#include "../tools/tools.h"
#include "../text/txt.h"
#include "../button/button.h"
#include "../input/input.h"
#include "../slider/slider.h"
#include "../my_input/my_input.h"

void Init_Menu(Menu *menu) {
    menu->buttonlist = NULL;
    menu->selected_index = -1;
    menu->b_ct = 0;
    menu->txtlist = NULL;
    menu->txt_ct = 0;
    menu->background = NULL;
    menu->t_margine = 0;
    menu->b_margine = 0;
    menu->slider_list = NULL;
    menu->s_ct = 0;
    menu->s_margine = 0;
    menu->input_list = NULL;
    menu->i_ct = 0;
    menu->i_margine = 0;
    menu->my_inputlist = NULL;
}

void render_menu(Game *game, Menu *menu) {
    if (menu->background != NULL) {
        SDL_BlitSurface(menu->background, NULL, game->screen, NULL);
    }
    render_textlist(game, menu->txtlist, menu->txt_ct);
    render_buttons(game, menu->buttonlist, menu->b_ct);
    render_sliders(game, menu->slider_list, menu->s_ct);
    render_my_inputs(game, menu->my_inputlist, menu->i_ct);
}

void node_Init(M_node *node, Menu *menu, int id) {
    node->menu = menu;
    node->id = id;
    node->back = NULL;
    node->child_list = NULL;
}

void free_menu(Menu *menu) {
    if (menu->buttonlist) {
        for (int i = 0; i < menu->b_ct; i++) {
            if (menu->buttonlist[i].not_hovered) SDL_FreeSurface(menu->buttonlist[i].not_hovered);
            if (menu->buttonlist[i].hovered) SDL_FreeSurface(menu->buttonlist[i].hovered);
            if (menu->buttonlist[i].basic) SDL_FreeSurface(menu->buttonlist[i].basic);
            if (menu->buttonlist[i].txt.surf) SDL_FreeSurface(menu->buttonlist[i].txt.surf);
        }
        free(menu->buttonlist);
    }
    if (menu->txtlist) {
        for (int i = 0; i < menu->txt_ct; i++) {
            if (menu->txtlist[i].surf) SDL_FreeSurface(menu->txtlist[i].surf);
        }
        free(menu->txtlist);
    }
    if (menu->slider_list) {
        for (int i = 0; i < menu->s_ct; i++) {
            free_slider(&menu->slider_list[i]);
        }
        free(menu->slider_list);
    }
    if (menu->input_list) {
        for (int i = 0; i < menu->i_ct; i++) {
            free_input_box(&menu->input_list[i]);
        }
        free(menu->input_list);
    }
    if (menu->my_inputlist) {
        for (int i = 0; i < menu->i_ct; i++) {
            if (menu->my_inputlist[i].not_hovered) SDL_FreeSurface(menu->my_inputlist[i].not_hovered);
            if (menu->my_inputlist[i].hovered) SDL_FreeSurface(menu->my_inputlist[i].hovered);
            if (menu->my_inputlist[i].active) SDL_FreeSurface(menu->my_inputlist[i].active);
            if (menu->my_inputlist[i].txt.surf) SDL_FreeSurface(menu->my_inputlist[i].txt.surf);
        }
        free(menu->my_inputlist);
    }
    if (menu->background) SDL_FreeSurface(menu->background);
}