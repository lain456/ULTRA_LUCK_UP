#include "init.h"
#include "tools/tools.h"

void Ini_Game(Game *game) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_DOUBLEBUF) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        exit(1);
    }

    printf("game running..\n");

    game->frame = 0;
    game->cycle = CYCLE;
    game->fullscreen = 0;
    game->released_mouse = 0;
    game->mouse_pressed = 0;
    game->height = HEIGHT;
    game->width = WIDTH;
    game->title = "LUCK UP";
    game->state = 1;
    game->quite = 0;
    game->music_volume = 10;
    game->fps = FPS;
    game->screen = SDL_SetVideoMode(game->width, game->height, 32, SDL_SWSURFACE | SDL_RESIZABLE);
    if (!game->screen) {
        printf("SDL_SetVideoMode failed: %s\n", SDL_GetError());
        exit(1);
    }

    game->mini_font = TTF_OpenFont(FREDOKA_PATH, 20);
    game->main_font = TTF_OpenFont(FREDOKA_PATH, 28);
    game->mid_font = TTF_OpenFont(FREDOKA_PATH, 30);
    game->big_main_font = TTF_OpenFont(FREDOKA_PATH, 45);

    if (!game->main_font) {
        printf("Error: Failed to load main_font: %s\n", TTF_GetError());
        exit(1);
    }

    if (!game->mini_font || !game->main_font || !game->mid_font || !game->big_main_font) {
        printf("Failed to load fonts: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    game->x_button_size = 200;
    game->y_button_size = 100;

    game->b_purple = IMG_Load(BUTTON_PNG_PATH);
    game->b_yellow = IMG_Load(BUTTON_PNG2_PATH);
    game->black_surface = create_color_surface(WIDTH, HEIGHT, 0, 0, 0);
    if (!game->b_purple || !game->b_yellow) {
        printf("Failed to load button images: %s\n", IMG_GetError());
        if (game->b_purple) SDL_FreeSurface(game->b_purple);
        if (game->b_yellow) SDL_FreeSurface(game->b_yellow);
        TTF_CloseFont(game->mini_font);
        TTF_CloseFont(game->main_font);
        TTF_CloseFont(game->mid_font);
        TTF_CloseFont(game->big_main_font);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    game->current_menu = NULL;
    game->slider_bg_color = (SDL_Color){91, 29, 191};
    game->slider_fill_color = (SDL_Color){250, 250, 150};
    game->slider_bg = NULL;
    game->slider_fill = NULL;
    game->b_slider = NULL;
    game->x_slider_size = 250;
    game->y_slider_size = 20;

    game->music = NULL;
    game->music_volume = 5;
    game->sfx_volume = 50;
    game->sfx = Mix_LoadWAV(HOVER_SFX_PATH);

    game->last_time_update = 0;
    game->time_string[0] = '\0';
    game->last_fps_update = SDL_GetTicks();
    game->frame_count = 0;
    game->current_fps = 0.0f;

    // Initialize new fields
    game->selected_button_index = -1;
    game->last_joystick_nav = 0;
    game->joystick_nav_cooldown = 200;
    game->select = 0;
    game->controller_active = 0;
    game->last_mouse_motion = 0;
    game->last_frame_time = SDL_GetTicks();

    // Initialize arduino_status to avoid garbage values
    game->arduino_status.B9 = 0;
    game->arduino_status.B10 = 0;
    game->arduino_status.B11 = 0;
    game->arduino_status.B12 = 0;
    game->arduino_status.B13 = 0;
    game->arduino_status.JX = 512; // Center value
    game->arduino_status.JY = 512; // Center value
    game->arduino_status.JSW = 0;
    strcpy(game->arduino_status.Green, "OFF");
    strcpy(game->arduino_status.Red, "OFF");
    game->prev_arduino_status = game->arduino_status;

    // Initialize default controls
    const char *default_controls[] = {
        "space", "q", "d", "z", "s", // P1: jump, left, right, up, down
        "enter", "[4]", "[6]", "[8]", "[5]", // P2: jump, left, right, up, down
        "up", "down", "escape", "enter", // Navigation: menu_up, menu_down, menu_escape, menu_select
        "e", "[0]" // Dash: P1, P2
    };

    strncpy(game->controls_p1.jump, default_controls[0], sizeof(game->controls_p1.jump));
    strncpy(game->controls_p1.left, default_controls[1], sizeof(game->controls_p1.left));
    strncpy(game->controls_p1.right, default_controls[2], sizeof(game->controls_p1.right));
    strncpy(game->controls_p1.up, default_controls[3], sizeof(game->controls_p1.up));
    strncpy(game->controls_p1.down, default_controls[4], sizeof(game->controls_p1.down));
    strncpy(game->controls_p2.jump, default_controls[5], sizeof(game->controls_p2.jump));
    strncpy(game->controls_p2.left, default_controls[6], sizeof(game->controls_p2.left));
    strncpy(game->controls_p2.right, default_controls[7], sizeof(game->controls_p2.right));
    strncpy(game->controls_p2.up, default_controls[8], sizeof(game->controls_p2.up));
    strncpy(game->controls_p2.down, default_controls[9], sizeof(game->controls_p2.down));
    strncpy(game->controls_p1.menu_up, default_controls[10], sizeof(game->controls_p1.menu_up));
    strncpy(game->controls_p1.menu_down, default_controls[11], sizeof(game->controls_p1.menu_down));
    strncpy(game->controls_p1.menu_escape, default_controls[12], sizeof(game->controls_p1.menu_escape));
    strncpy(game->controls_p1.menu_select, default_controls[13], sizeof(game->controls_p1.menu_select));
    strncpy(game->controls_p1.dash, default_controls[14], sizeof(game->controls_p1.dash));
    strncpy(game->controls_p2.dash, default_controls[15], sizeof(game->controls_p2.dash));

    // Copy navigation controls to player 2
    strncpy(game->controls_p2.menu_up, default_controls[10], sizeof(game->controls_p2.menu_up));
    strncpy(game->controls_p2.menu_down, default_controls[11], sizeof(game->controls_p2.menu_down));
    strncpy(game->controls_p2.menu_escape, default_controls[12], sizeof(game->controls_p2.menu_escape));
    strncpy(game->controls_p2.menu_select, default_controls[13], sizeof(game->controls_p2.menu_select));

    printf("Default controls initialized for P1 and P2\n");











    // lvls and shit
    game->lvl_x_size = WIDTH;
    game->lvl_y_size = HEIGHT;
}