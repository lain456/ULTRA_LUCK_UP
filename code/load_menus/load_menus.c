#include <stdio.h>
#include <stdlib.h>
#include "load_menus.h"
#include "../game/game.h"
#include "../tools/tools.h"
#include "../slider/slider.h"

#include "../my_input/my_input.h"

Menu play_menu(Game game) {
    Menu play;
    Init_Menu(&play);

    play.b_ct = 5;
    play.txt_ct = 3;
    play.t_margine = 10;
    play.b_margine = 10;

    play.buttonlist = (Button *)malloc(sizeof(Button) * play.b_ct);
    play.txtlist = (Text *)malloc(sizeof(Text) * play.txt_ct);

    char *button_strings[] = {"play", "options", "enigme", "quit", "help"};
    for (int i = 0; i < play.b_ct; i++) {
        play.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) / 2, 300, game.y_button_size, game.x_button_size, button_strings[i], WHITE, 1);
    }

    y_order_buttons(play.buttonlist, play.b_margine, play.b_ct);

    b_pos_update(&play.buttonlist[4], game.width - game.x_button_size, game.height - game.y_button_size + play.b_margine);

    play.txtlist[0] = *create_txt(" M A I N    G A M E ", game.big_main_font, BLACK, 20, 20);
    play.txtlist[1] = *create_txt("presented by lain", game.main_font, BLACK, play.txtlist[0].rect.x, play.txtlist[0].rect.y + play.txtlist[0].rect.h + play.t_margine);

    return play;
}

Menu exit_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);

    new_menu.b_ct = 2;
    new_menu.txt_ct = 1;
    new_menu.t_margine = 10;
    new_menu.b_margine = 250;
    char *button_strings[] = {"no", "yes"};

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);

    for (int i = 0; i < new_menu.b_ct; i++) {
        new_menu.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) * 5 / 20, (HEIGHT - game.y_button_size) * 3 / 4, game.y_button_size, game.x_button_size, button_strings[i], WHITE, 1);
    }

    x_order_buttons(new_menu.buttonlist, new_menu.b_margine, new_menu.b_ct);

    new_menu.txtlist[0] = *create_txt(" you sure you wanna leave ? ", game.big_main_font, BLACK, WIDTH * 1 / 5, HEIGHT * 2 / 5);

    return new_menu;
}

Menu options_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);

    new_menu.b_ct = 4;
    new_menu.txt_ct = 1;
    new_menu.s_ct = 2;
    new_menu.t_margine = 10;
    new_menu.b_margine = 25;
    new_menu.s_margine = 25;
    char *button_strings[] = {"music : on", "sfx : on", "fullscreen : on", "return"};

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);
    new_menu.slider_list = (Slider *)malloc(sizeof(Slider) * new_menu.s_ct);

    for (int i = 0; i < new_menu.b_ct; i++) {
        new_menu.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) * 12 / 32, (HEIGHT - game.y_button_size) * 13 / 32, game.y_button_size, game.x_button_size, button_strings[i], WHITE, 1);
    }

    y_order_buttons(new_menu.buttonlist, new_menu.b_margine, new_menu.b_ct);

    new_menu.txtlist[0] = *create_txt("controls... ", game.big_main_font, BLACK, WIDTH * 2 / 5, HEIGHT * 1 / 5);

    Slider s = *create_slider(&game, (SDL_Rect){(WIDTH - game.x_button_size) * 12 / 32 + game.x_button_size + new_menu.s_margine, (HEIGHT - game.y_slider_size) * 27 / 64, game.x_slider_size, game.y_slider_size});
    new_menu.slider_list[0] = s;
    new_menu.slider_list[1] = *create_slider(&game, (SDL_Rect){(WIDTH - game.x_button_size) * 12 / 32 + game.x_button_size + new_menu.s_margine, new_menu.buttonlist[1].b_rect.y + (new_menu.buttonlist[1].b_rect.h - game.y_slider_size) / 2, game.x_slider_size, game.y_slider_size});

    return new_menu;
}

Menu WIP_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);
    new_menu.background = create_color_surface(game.width, game.height, 0, 0, 0);

    new_menu.b_ct = 1;
    new_menu.txt_ct = 1;
    new_menu.t_margine = 10;
    new_menu.b_margine = 25;
    char *button_strings[] = {"return"};

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);

    for (int i = 0; i < new_menu.b_ct; i++) {
        new_menu.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) * 10 / 20, (HEIGHT - game.y_button_size) * 3 / 4, game.y_button_size, game.x_button_size, button_strings[i], WHITE, 1);
    }

    new_menu.txtlist[0] = *create_txt(" we are still working on this... ", game.big_main_font, WHITE, WIDTH * 1 / 5, HEIGHT * 2 / 5);

    return new_menu;
}

Menu player_choice_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);

    new_menu.b_ct = 2;
    new_menu.txt_ct = 1;
    new_menu.t_margine = 10;
    new_menu.b_margine = 250;
    char *button_strings[] = {"yellow", "purple"};

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);

    for (int i = 0; i < new_menu.b_ct; i++) {
        new_menu.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) * 5 / 20, (HEIGHT - game.y_button_size) * 3 / 4, game.y_button_size, game.x_button_size, button_strings[i], WHITE, 1);
    }

    x_order_buttons(new_menu.buttonlist, new_menu.b_margine, new_menu.b_ct);

    new_menu.txtlist[0] = *create_txt(" which color do you like ? ", game.big_main_font, BLACK, WIDTH * 1 / 5, HEIGHT * 2 / 5);

    return new_menu;
}

Menu multiplayer_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);

    new_menu.b_ct = 2;
    new_menu.txt_ct = 1;
    new_menu.t_margine = 10;
    new_menu.b_margine = 250;
    char *button_strings[] = {"NUH UHH", "HELL YEAH"};

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);

    for (int i = 0; i < new_menu.b_ct; i++) {
        new_menu.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) * 5 / 20, (HEIGHT - game.y_button_size) * 3 / 4, game.y_button_size, game.x_button_size, button_strings[i], GOLD, 1);
    }

    x_order_buttons(new_menu.buttonlist, new_menu.b_margine, new_menu.b_ct);

    new_menu.txtlist[0] = *create_txt("        do you have bitches ? ", game.big_main_font, BLACK, WIDTH * 1 / 5, HEIGHT * 2 / 5);

    return new_menu;
}




// ... Other menu functions unchanged ...

Menu help_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);
    new_menu.background = create_color_surface(game.width, game.height, 0, 0, 0);

    new_menu.b_ct = 1;
    new_menu.txt_ct = 1;
    new_menu.i_ct = 1;
    new_menu.t_margine = 10;
    new_menu.b_margine = 25;

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);
    new_menu.my_inputlist = (My_input *)malloc(sizeof(My_input) * new_menu.i_ct);

    new_menu.buttonlist[0] = *create_button(&game, (WIDTH - game.x_button_size) * 10 / 20, (HEIGHT - game.y_button_size) * 3 / 4, game.y_button_size, game.x_button_size, "return", WHITE, 1);

    new_menu.txtlist[0] = *create_txt(" enter your name.. ", game.big_main_font, WHITE, (WIDTH - 300) / 2, HEIGHT * 2 / 5);

    InputStyle style = {
        .not_hovered_color = {0, 200, 0}, // Green
        .hovered_color = {0, 255, 0}, // Brighter green
        .active_color = {100, 255, 100} // Light green
    };
    init_my_input(&game, &new_menu.my_inputlist[0], (WIDTH - 150) / 2, (HEIGHT - 70) * 3 / 5, 150, 70, &style);

    return new_menu;
}

// ... Other menu functions unchanged ...

Menu difficulty_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);

    new_menu.b_ct = 4;
    new_menu.txt_ct = 1;
    new_menu.t_margine = 10;
    new_menu.b_margine = 25;
    char *button_strings[] = {"Easy (2x2)", "Medium (4x4)", "Hard (16x16)", "Return"};

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);

    for (int i = 0; i < new_menu.b_ct; i++) {
        new_menu.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) / 2, (HEIGHT - game.y_button_size) * 13 / 32, game.y_button_size, game.x_button_size, button_strings[i], WHITE, 1);
    }

    y_order_buttons(new_menu.buttonlist, new_menu.b_margine, new_menu.b_ct);

    new_menu.txtlist[0] = *create_txt("Select Puzzle Difficulty", game.big_main_font, BLACK, WIDTH * 2 / 5, HEIGHT * 1 / 5);

    return new_menu;
}