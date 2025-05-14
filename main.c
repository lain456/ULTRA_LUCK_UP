#include "init/init.h"
#include "game/game.h"
#include "load_menus/load_menus.h"
#include "tools/tools.h"
#include "Player/player.h"
#include "Background/background.h"
#include "button/button.h"
#include "slider/slider.h"
#include "audio/audio.h"
#include "text/txt.h"
#include "menu/menu.h"
#include "arduino/arduino.h"
#include "shity_code/shity_code.h"

int main(int argc, char *argv[]) {
    Game game;
    Ini_Game(&game);
    load_background(&game);
    game.music = loadMusic(MUSIC_PATH);
    game.sfx = Mix_LoadWAV(HOVER_SFX_PATH);
    Mix_PlayMusic(game.music, -1);

    // Initialize player before menus
    game.player = malloc(sizeof(Player));
    if (!game.player) {
        printf("Error: Failed to allocate player\n");
        SDL_Quit();
        return 1;
    }
    memset(game.player, 0, sizeof(Player));
    game.player->player_num = 0; // Default to yellow bird
    initPlayer(game.player);
    printf("Player initialized in main (player_num=%d)\n", game.player->player_num);

    // Attempt to initialize serial port for Arduino (optional)
    if (!init_serial(&game, "/dev/ttyUSB0")) {
        printf("Serial initialization failed, continuing with keyboard input\n");
        game.controller_active = 0;
    } else {
        printf("Serial initialized, but disabling controller for debugging\n");
        game.controller_active = 0;
    }

    M_node n, n0, n1, n2, n3;
    Menu play = play_menu(game);
    Menu exit = exit_menu(game);
    Menu options = options_menu(game);
    Menu wip = WIP_menu(game);
    Menu player_choice = player_choice_menu(game);

    node_Init(&n0, &exit, 0);
    node_Init(&n1, &play, 1);
    node_Init(&n2, &options, 2);
    node_Init(&n3, &player_choice, 3);
    node_Init(&n, &wip, -1);

    n0.back = &n1;
    n1.back = &n0;
    n2.back = &n1;
    n.back = &n1;
    n3.back = &n1;

    game.current_node = &n1;
    game.current_menu = n1.menu;

    while (!game.quite) {
        Uint32 frame_start = SDL_GetTicks();

        SDL_GetMouseState(&game.x_mouse, &game.y_mouse);
        game.released_mouse = 0;
        game.mouse_pressed = 0;

        while (SDL_PollEvent(&game.event)) {
            switch (game.event.type) {
                case SDL_KEYDOWN:
                    if (game.event.key.keysym.sym == SDLK_ESCAPE && game.state == 1) {
                        game.current_node = game.current_node->back;
                        game.selected_button_index = -1;
                        game.select = 0;
                        game.controller_active = 0;
                    }
                    break;
                case SDL_QUIT:
                    game.quite = 1;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (game.event.button.button == SDL_BUTTON_LEFT) {
                        game.mouse_pressed = 1;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (game.event.button.button == SDL_BUTTON_LEFT) {
                        game.released_mouse = 1;
                        game.mouse_pressed = 0;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    game.last_mouse_motion = SDL_GetTicks();
                    game.controller_active = 0;
                    game.select = 0;
                    break;
            }
        }

        switch (game.state) {
            case 1:
                update_buttons(&game, game.current_node->menu->buttonlist, game.current_node->menu->b_ct);

                switch (game.current_node->id) {
                    case -1: // WIP menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        break;
                    case 0: // Exit menu
                        if (game.current_node->menu->buttonlist[0].isClicked) { // No
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) { // Yes
                            game.quite = 1;
                        }
                        break;
                    case 1: // Play menu
                        if (game.current_node->menu->buttonlist[0].isClicked) { // Play
                            printf("choosing player..\n");
                            game.current_node = &n3;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) { // Options
                            game.current_node = &n2;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[2].isClicked ||
                            game.current_node->menu->buttonlist[4].isClicked) { // Info or Help
                            game.current_node = &n;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[3].isClicked) { // Quit
                            game.current_node = &n0;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        break;
                    case 2: // Options menu
                        if (game.current_node->menu->buttonlist[0].isClicked) { // Music toggle
                            update_txt(&game.current_node->menu->txtlist[0], ". . . ", BLACK, game.big_main_font);
                            game.music_volume = (game.music_volume == 0) ? 69 : 0;
                            update_txt(&game.current_node->menu->buttonlist[0].txt,
                                      game.music_volume != 0 ? "music on" : "music off", GOLD, NULL);
                            break;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) { // SFX toggle
                            game.sfx_volume = (game.sfx_volume == 0) ? 69 : 0;
                            update_txt(&game.current_node->menu->buttonlist[1].txt,
                                      game.sfx_volume != 0 ? "sfx : on" : "sfx : off", GOLD, NULL);
                            break;
                        }
                        if (game.current_node->menu->buttonlist[2].isClicked) { // Fullscreen toggle
                            toggle_fullscreen(&game);
                            update_txt(&game.current_node->menu->buttonlist[2].txt,
                                      game.fullscreen ? "fullscreen off" : "fullscreen on", GOLD, game.mini_font);
                            break;
                        }
                        if (game.current_node->menu->buttonlist[3].isClicked) { // Return
                            update_txt(&game.current_node->menu->txtlist[0], " welcome back :D", BLACK, game.big_main_font);
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        update_slider(&game, &game.current_node->menu->slider_list[0], game.music_volume);
                        update_slider(&game, &game.current_node->menu->slider_list[1], game.sfx_volume);
                        game.music_volume = game.current_node->menu->slider_list[0].val;
                        game.sfx_volume = game.current_node->menu->slider_list[1].val;
                        break;
                    case 3: // Player choice menu
                        if (game.current_node->menu->buttonlist[1].isClicked) { // Purple bird
                            game.player->player_num = 1;
                            game.current_node = &n;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            game.state = 0;
                            printf("Selected purple bird, player_num=%d\n", game.player->player_num);
                            initPlayer(game.player); // Reinitialize to update sprite selection
                            break;
                        }
                        if (game.current_node->menu->buttonlist[0].isClicked) { // Yellow bird
                            game.player->player_num = 0;
                            game.current_node = &n;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            game.state = 0;
                            printf("Selected yellow bird, player_num=%d\n", game.player->player_num);
                            initPlayer(game.player); // Reinitialize to update sprite selection
                            break;
                        }
                        break;
                    default:
                        printf("Invalid menu ID\n");
                        break;
                }

                render_background(&game);
                if (game.state) render_menu(&game, game.current_node->menu);
                break;
            case 0:
                gameplay(&game);
                break;
        }

        SDL_Flip(game.screen);
        Mix_VolumeMusic(MIX_MAX_VOLUME * game.music_volume / 100);
        Mix_VolumeChunk(game.sfx, MIX_MAX_VOLUME * game.sfx_volume / 100);

        // FPS control
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        Uint32 frame_duration = 1000 / game.fps;
        if (frame_time < frame_duration) {
            SDL_Delay(frame_duration - frame_time);
        }
        game.last_frame_time = SDL_GetTicks();
    }

    // Cleanup
    if (game.player) {
        freePlayer(game.player);
        free(game.player);
        game.player = NULL;
    }


    SDL_Quit();
    return 0;
}