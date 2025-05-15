#include "menu.h"
#include "../slider/slider.h"
#include "../input/input.h"

void Init_Menu(Menu *menu)
{
    menu->b_ct = 0;
    menu->txt_ct = 0;
    menu->s_margine = 0;
    menu->s_ct = 0;
    menu->slider_list = NULL;
    menu->b_margine = 0;
    menu->t_margine = 0;
    menu->buttonlist = NULL;
    menu->txtlist = NULL;
    menu->background = NULL;
    menu->i_ct = 0;
    menu->input_list = NULL;
}

void render_menu(Game *game, Menu *menu)
{
    if (menu->background)
    {
        SDL_BlitSurface(menu->background, NULL, game->screen, NULL);
    }

    if (menu->slider_list)
    {
        for (int i = 0; i < menu->s_ct; i++)
        {
            render_slider(game, &menu->slider_list[i]);
        }
    }

    if (menu->buttonlist)
    {
        render_buttons(game, menu->buttonlist, menu->b_ct);
    }
    else
    {
        printf("error: b_list is null\n");
    }

    if (menu->txtlist)
    {
        render_textlist(game, menu->txtlist, menu->txt_ct);
    }
    else
    {
        printf("error: txtlist is null\n");
    }

    if (menu->input_list)
    {
        for (int i = 0; i < menu->i_ct; i++)
        {
            render_input_box(game, &menu->input_list[i]);
        }
    }
}

M_node *M_link_Node(Menu *menu, M_node *parent)
{
    M_node *node = (M_node *)malloc(sizeof(M_node));
    if (node == NULL)
    {
        printf("Error in M_Create_Node\n");
        return NULL;
    }
    node->back = parent;
    node->menu = menu;
    node->child_list = (M_node *)malloc(sizeof(M_node) * menu->b_ct);
    return node;
}

void node_Init(M_node *node, Menu *menu, int id)
{
    node->child_list = (M_node *)malloc(sizeof(M_node) * menu->b_ct);
    node->menu = menu;
    node->id = id;
    node->back = NULL;
}