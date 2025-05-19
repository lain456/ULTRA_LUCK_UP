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
    //play.background = scaleSurface(IMG_Load(GOOD_BG),game.width ,game.height);

    play.b_ct = 5; // Added Controls button
    play.txt_ct = 3;
    play.t_margine = 10;
    play.b_margine = 10;

    play.buttonlist = (Button *)malloc(sizeof(Button) * play.b_ct);
    play.txtlist = (Text *)malloc(sizeof(Text) * play.txt_ct);

    char *button_strings[] = {"play", "options", "enigme", "exit", "help", };
    for (int i = 0; i < play.b_ct; i++) {
        play.buttonlist[i] = *create_button(&game, (WIDTH - game.x_button_size) * 5/ 10,
                                           (HEIGHT - game.y_button_size) * 5/ 10 ,
                                           game.y_button_size, game.x_button_size,
                                           button_strings[i], WHITE, 1);
    }

    y_order_buttons(play.buttonlist, play.b_margine, play.b_ct);

    play.txtlist[0] = *create_txt("Welcome to Ultra Luck Up!", game.big_main_font, WHITE, (WIDTH - 300) / 2, HEIGHT * 100 / 5);
    play.txtlist[1] = *create_txt("Choose an option", game.main_font, WHITE, (WIDTH - 200) / 2, HEIGHT * 1 / 10);
    play.txtlist[2] = *create_txt("Use mouse or arrows to navigate", game.main_font, WHITE, (WIDTH - 300) / 2, HEIGHT * 2 / 10);

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

    new_menu.b_ct = 5;
    new_menu.txt_ct = 1;
    new_menu.s_ct = 2;
    new_menu.t_margine = 10;
    new_menu.b_margine = 25;
    new_menu.s_margine = 25;
    char *button_strings[] = {"music : on", "sfx : on", "fullscreen : on","controls" ,  "return"};

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

Menu controls_menu(Game game) {
    Menu new_menu;
    Init_Menu(&new_menu);
    new_menu.background = scaleSurface(IMG_Load(GOOD_BG),game.width ,game.height);

    new_menu.b_ct = 3; // Return, Save, Reset
    new_menu.txt_ct = 17; // Titles, labels, and navigation header
    new_menu.i_ct = 14; // 5 P1, 5 P2, 4 navigation
    new_menu.t_margine = 5;
    new_menu.b_margine = 30; // Increased for better spacing

    new_menu.buttonlist = (Button *)malloc(sizeof(Button) * new_menu.b_ct);
    new_menu.txtlist = (Text *)malloc(sizeof(Text) * new_menu.txt_ct);
    new_menu.my_inputlist = (My_input *)malloc(sizeof(My_input) * new_menu.i_ct);

    // Buttons: Spaced horizontally using b_margine
    int total_button_width = new_menu.b_ct * game.x_button_size + (new_menu.b_ct - 1) * new_menu.b_margine;
    int start_x = (WIDTH - total_button_width) / 2;
    new_menu.buttonlist[0] = *create_button(&game, start_x, HEIGHT * 8 / 10 +50, game.y_button_size , game.x_button_size, "return", WHITE, 1);
    new_menu.buttonlist[1] = *create_button(&game, start_x + game.x_button_size + new_menu.b_margine, HEIGHT * 8 / 10 +50, game.y_button_size  , game.x_button_size, "save", WHITE, 1);
    new_menu.buttonlist[2] = *create_button(&game, start_x + 2 * (game.x_button_size + new_menu.b_margine), HEIGHT * 8 / 10 +50, game.y_button_size , game.x_button_size, "reset", WHITE, 1);

    // Text labels
    new_menu.txtlist[0] = *create_txt("Player 1 Controls", game.big_main_font, WHITE, (WIDTH - 400) / 2 - 100, HEIGHT * 1 / 12);
    new_menu.txtlist[1] = *create_txt("Jump:", game.mid_font, WHITE, (WIDTH - 400) / 2 - 100, HEIGHT * 2 / 12);
    new_menu.txtlist[2] = *create_txt("Left:", game.mid_font, WHITE, (WIDTH - 400) / 2 - 100, HEIGHT * 3 / 12);
    new_menu.txtlist[3] = *create_txt("Right:", game.mid_font, WHITE, (WIDTH - 400) / 2 - 100, HEIGHT * 4 / 12);
    new_menu.txtlist[4] = *create_txt("Up:", game.mid_font, WHITE, (WIDTH - 400) / 2 - 100, HEIGHT * 5 / 12);
    new_menu.txtlist[5] = *create_txt("Down:", game.mid_font, WHITE, (WIDTH - 400) / 2 - 100, HEIGHT * 6 / 12);

    new_menu.txtlist[6] = *create_txt("Player 2 Controls", game.big_main_font, WHITE, (WIDTH + 100) / 2  +100 , HEIGHT * 1 / 12);
    new_menu.txtlist[7] = *create_txt("Jump:", game.mid_font, WHITE, (WIDTH + 100) / 2  +100 , HEIGHT * 2 / 12);
    new_menu.txtlist[8] = *create_txt("Left:", game.mid_font, WHITE, (WIDTH + 100) / 2  +100 , HEIGHT * 3 / 12);
    new_menu.txtlist[9] = *create_txt("Right:", game.mid_font, WHITE, (WIDTH + 100) / 2  +100 , HEIGHT * 4 / 12);
    new_menu.txtlist[10] = *create_txt("Up:", game.mid_font, WHITE, (WIDTH + 100) / 2  +100 , HEIGHT * 5 / 12);
    new_menu.txtlist[11] = *create_txt("Down:", game.mid_font, WHITE, (WIDTH + 100) / 2  +100 , HEIGHT * 6 / 12);

    new_menu.txtlist[12] = *create_txt("Navigation Controls", game.big_main_font, WHITE, (WIDTH - 400) / 2, HEIGHT * 7 / 12  +50);
    new_menu.txtlist[13] = *create_txt("Menu Up:", game.mid_font, WHITE, (WIDTH - 400) / 2, HEIGHT * 8 / 12 +50);
    new_menu.txtlist[14] = *create_txt("Menu Down:", game.mid_font, WHITE, (WIDTH - 400) / 2, HEIGHT * 9 / 12 +50);
    new_menu.txtlist[15] = *create_txt("back :", game.mid_font, WHITE, (WIDTH + 400) / 2 , HEIGHT * 8 / 12 +50);
    new_menu.txtlist[16] = *create_txt("next :", game.mid_font, WHITE, (WIDTH + 400 ) / 2 , HEIGHT * 9 / 12 +50);

    InputStyle style = {
        .not_hovered_color = YELLOW,
        .hovered_color = LIGHT_YELLOW,
        .active_color = WHITE
    };

    // Input boxes (smaller: 80x30)
    int input_width = 95, input_height = 30;
    // P1 controls
    init_my_input(&game, &new_menu.my_inputlist[0], (WIDTH - 500) / 2, HEIGHT * 2 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[1], (WIDTH - 500) / 2, HEIGHT * 3 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[2], (WIDTH - 500) / 2, HEIGHT * 4 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[3], (WIDTH - 500) / 2, HEIGHT * 5 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[4], (WIDTH - 500) / 2, HEIGHT * 6 / 12, input_width, input_height, &style);
    // P2 controls
    init_my_input(&game, &new_menu.my_inputlist[5], (WIDTH + 200) / 2, HEIGHT * 2 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[6], (WIDTH + 200) / 2, HEIGHT * 3 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[7], (WIDTH + 200) / 2, HEIGHT * 4 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[8], (WIDTH + 200) / 2, HEIGHT * 5 / 12, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[9], (WIDTH + 200) / 2, HEIGHT * 6 / 12, input_width, input_height, &style);
    // Navigation controls
    init_my_input(&game, &new_menu.my_inputlist[10], (WIDTH - 300) / 2 -100, HEIGHT * 8 / 12 +50, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[11], (WIDTH - 300) / 2 -100, HEIGHT * 9 / 12 +50, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[12], (WIDTH - 100) / 2 + 100, HEIGHT * 8 / 12 +50, input_width, input_height, &style);
    init_my_input(&game, &new_menu.my_inputlist[13], (WIDTH - 100) / 2 + 100, HEIGHT * 9 / 12 +50, input_width, input_height, &style);

    // Default values with numpad support for P2
    const char *defaults[] = {
        "space", "q", "d", "z", "s", // P1: WASD + Space
        "kpenter", "kp4", "kp6", "kp8", "kp5", // P2: Numpad
        "up", "down", "escape" , "enter" // Navigation
    };
    for (int i = 0; i < new_menu.i_ct; i++) {
        if (i <5)
        {
            new_menu.my_inputlist[i].rect.x -= 100;
        }


        new_menu.my_inputlist[i].rect.x = new_menu.my_inputlist[i].rect.x + 80 ;
        new_menu.my_inputlist[i].rect.y += -20;

        snprintf(new_menu.my_inputlist[i].value, sizeof(new_menu.my_inputlist[i].value), "%s", defaults[i]);
        update_input_text(&new_menu.my_inputlist[i]); // Render default text
        sync_input(&game, &new_menu.my_inputlist[i], i); // Sync to controls
    }
    for (int i = 0; i < new_menu.txt_ct; i++)
    {
        new_menu.txtlist[i].rect.y += -30 ;
        new_menu.txtlist[i].rect.x += -200 ;
        if (i< 12)
        {
            new_menu.txtlist[i].rect.x += 100 ;

        }
    }




    return new_menu;
}