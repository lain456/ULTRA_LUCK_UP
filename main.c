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
#include "my_input/my_input.h"

int main(int argc, char *argv[]) {
    Game game;
    Ini_Game(&game);

    // Enable Unicode input for text
    SDL_EnableUNICODE(1);

    // Verify font initialization
    if (!game.main_font) {
        printf("Error: game->main_font is NULL after Ini_Game\n");
    } else {
        printf("Font initialized: game->main_font=%p\n", (void*)game.main_font);
    }

    load_background(&game);
    game.music = loadMusic(MUSIC_PATH);
    game.sfx = Mix_LoadWAV(HOVER_SFX_PATH);
    Mix_PlayMusic(game.music, -1);

    game.player = malloc(sizeof(Player));
    if (!game.player) {
        printf("Error: Failed to allocate player\n");
        SDL_Quit();
        return 1;
    }
    game.player->player_num = 0;
    initPlayer(game.player);
    printf("Player initialized: player_num=%d\n", game.player->player_num);

    game.player2 = NULL;
    game.platform_count = 0;
    game.platforms = NULL;

    if (!init_serial(&game, "/dev/ttyUSB0")) {
        printf("Error: Failed to initialize serial port. Continuing with keyboard input.\n");
        game.controller_active = 0;
    } else {
        printf("Serial port initialized, but controller disabled for debugging.\n");
        game.controller_active = 0;
    }

    M_node n, n0, n1, n2, n3, n4, n5, n6,n7;
    Menu play = play_menu(game);
    Menu exit = exit_menu(game);
    Menu options = options_menu(game);
    Menu wip = WIP_menu(game);
    Menu player_choice = player_choice_menu(game);
    Menu multiplayer = multiplayer_menu(game);
    Menu difficulty = difficulty_menu(game);
    Menu help = help_menu(game);
    Menu control = controls_menu(game);

    node_Init(&n0, &exit, 0);
    node_Init(&n1, &play, 1);
    node_Init(&n2, &options, 2);
    node_Init(&n3, &player_choice, 3);
    node_Init(&n4, &multiplayer, 4);
    node_Init(&n5, &difficulty, 5);
    node_Init(&n6, &help, 6);
    node_Init(&n7, &control, 7);

    node_Init(&n, &wip, -1);


    n0.back = &n1;
    n1.back = &n0;
    n2.back = &n1;
    n.back = &n1;
    n3.back = &n1;
    n4.back = &n1;
    n5.back = &n1;
    n6.back = &n1;
    n7.back = &n2;

    game.current_node = &n7;
    game.current_menu = n1.menu;




    reset_controls(&game, n7.menu->my_inputlist);

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
            // Handle My_input events in help menu
            if (game.state == 1 && game.current_node->id == 6) {
                for (int i = 0; i < game.current_node->menu->i_ct; i++) {
                    handle_my_input_event(&game, &game.current_node->menu->my_inputlist[i], &game.event);
                }
            }
        }

        render_background(&game);

        switch (game.state) {
            case 0: // Gameplay
                gameplay(&game);
                break;
            case 1: // Menu
                update_buttons(&game, game.current_node->menu->buttonlist, game.current_node->menu->b_ct);
                switch (game.current_node->id) {
                    case -1: // WIP menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        break;
                    case 0: // Exit menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) {
                            game.quite = 1;
                        }
                        break;
                    case 1: // Play menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            printf("Choosing player: single or multiplayer\n");
                            game.current_node = &n4;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) {
                            game.current_node = &n2;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[2].isClicked) {
                            game.current_node = &n5;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[3].isClicked) {
                            game.current_node = &n0;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[4].isClicked) {
                            game.current_node = &n6;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }



                        break;
                    case 2: // Options menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            update_txt(&game.current_node->menu->txtlist[0], ". . . ", BLACK, game.big_main_font);
                            game.music_volume = (game.music_volume == 0) ? 69 : 0;
                            update_txt(&game.current_node->menu->buttonlist[0].txt,
                                      game.music_volume != 0 ? "music : on" : "music : off", GOLD, NULL);
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) {
                            game.sfx_volume = (game.sfx_volume == 0) ? 69 : 0;
                            update_txt(&game.current_node->menu->buttonlist[1].txt,
                                      game.sfx_volume != 0 ? "sfx : on" : "sfx : off", GOLD, NULL);
                        }
                        if (game.current_node->menu->buttonlist[2].isClicked) {
                            toggle_fullscreen(&game);
                            update_txt(&game.current_node->menu->buttonlist[2].txt,
                                      game.fullscreen ? "fullscreen : off" : "fullscreen : on", GOLD, game.mini_font);
                        }
                        if (game.current_node->menu->buttonlist[4].isClicked) {
                            update_txt(&game.current_node->menu->txtlist[0], " welcome back :D", BLACK, game.big_main_font);
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            break;
                        }
                        if (game.current_node->menu->buttonlist[3].isClicked)
                        {
                            game.current_node = &n7;
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
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            game.player->player_num = 0;
                            if (game.player2) {
                                game.player2->player_num = 1;
                                initPlayer(game.player2);
                            }
                            game.current_node = &n;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            game.state = 0;
                            printf("Selected yellow bird for player1, player_num=%d\n", game.player->player_num);
                            initPlayer(game.player);
                            break;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) {
                            game.player->player_num = 1;
                            if (game.player2) {
                                game.player2->player_num = 0;
                                initPlayer(game.player2);
                            }
                            game.current_node = &n;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                            game.state = 0;
                            printf("Selected purple bird for player1, player_num=%d\n", game.player->player_num);
                            initPlayer(game.player);
                            break;
                        }
                        break;
                    case 4: // Multiplayer menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            game.multiplayer = 0;
                            if (game.player2) {
                                freePlayer(game.player2);
                                free(game.player2);
                                game.player2 = NULL;
                            }
                            game.current_node = &n3;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) {
                            game.multiplayer = 1;
                            if (!game.player2) {
                                game.player2 = malloc(sizeof(Player));
                                if (!game.player2) {
                                    printf("Error: Failed to allocate player2\n");
                                    game.quite = 1;
                                    break;
                                }
                                game.player2->player_num = (game.player->player_num == 0) ? 1 : 0;
                                initPlayer(game.player2);
                                printf("Player2 allocated: player_num=%d\n", game.player2->player_num);
                            }
                            game.current_node = &n3;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        break;
                    case 5: // Difficulty menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            game.state = 2;
                            shity_function(&game, DIFFICULTY_EASY);
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        if (game.current_node->menu->buttonlist[1].isClicked) {
                            game.state = 2;
                            shity_function(&game, DIFFICULTY_MEDIUM);
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        if (game.current_node->menu->buttonlist[2].isClicked) {
                            game.state = 2;
                            shity_function(&game, DIFFICULTY_HARD);
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        if (game.current_node->menu->buttonlist[3].isClicked) {
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        break;
                    case 6: // Help menu
                        if (game.current_node->menu->buttonlist[0].isClicked) {
                            printf("Returning to main menu\n");
                            game.current_node = &n1;
                            game.selected_button_index = -1;
                            game.select = 0;
                            game.controller_active = 0;
                        }
                        break;

                    case 7:
                        {
                            for (int i = 0; i < game.current_node->menu->i_ct; i++) {
                                handle_my_input_event(&game, &game.current_node->menu->my_inputlist[i], &game.event);
                            }

                            if (game.current_node->menu->buttonlist[0].isClicked) { // Return
                                printf("Returning to main menu\n");
                                game.current_node = &n1;
                                game.selected_button_index = -1;
                                game.select = 0;
                                game.controller_active = 0;
                            }
                            if (game.current_node->menu->buttonlist[1].isClicked) { // Save
                                for (int i = 0; i < game.current_node->menu->i_ct; i++) {
                                    sync_input(&game, &game.current_node->menu->my_inputlist[i], i);
                                }
                                printf("Controls saved\n");
                            }
                            if (game.current_node->menu->buttonlist[2].isClicked) { // Reset
                                reset_controls(&game, game.current_node->menu->my_inputlist);
                                printf("Controls reset to defaults\n");
                            }
                            break;
                        }break;
                    default:
                        printf("Error: Invalid menu ID\n");
                        break;
                }
                render_menu(&game, game.current_node->menu);
                break;
            case 2: // Puzzle mode
                shity_function(&game, DIFFICULTY_MEDIUM);
                break;
        }

        SDL_Flip(game.screen);
        Mix_VolumeMusic(MIX_MAX_VOLUME * game.music_volume / 100);
        Mix_VolumeChunk(game.sfx, MIX_MAX_VOLUME * game.sfx_volume / 100);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        Uint32 frame_duration = 1000 / game.fps;
        if (frame_time < frame_duration) {
            SDL_Delay(frame_duration - frame_time);
        }
        game.last_frame_time = SDL_GetTicks();
    }

    if (game.player) {
        freePlayer(game.player);
        free(game.player);
        game.player = NULL;
    }
    if (game.player2) {
        freePlayer(game.player2);
        free(game.player2);
        game.player2 = NULL;
    }
    if (game.platforms) {
        free(game.platforms);
        game.platforms = NULL;
    }

    SDL_Quit();
    return 0;
}