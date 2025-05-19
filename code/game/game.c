#include "game.h"
#include "../tools/tools.h"
#include "../Player/player.h"
#include "../Background/background.h"
#include "init.h"

// Updates camera position based on player's position and background dimensions
// Returns the camera's x offset in world coordinates
int update_camera(int player_x, int screen_width, int bg_width, int player_width) {
    // Center the player in the viewport, adjusted for player width
    int desired_camera_x = player_x - screen_width / 2 + player_width / 2;

    // Clamp camera to background boundaries
    if (desired_camera_x < 0) {
        desired_camera_x = 0;
    }
    if (desired_camera_x > bg_width - screen_width) {
        desired_camera_x = bg_width - screen_width;
    }

    return desired_camera_x;
}

int gameplay(Game *game) {
    if (!game || !game->screen || !game->player) {
        printf("Error: Invalid game, screen, or player pointer\n");
        game->state = 1;
        return 0;
    }

    SDL_BlitSurface(create_color_surface(game->width, game->height, 200, 200, 200), NULL, game->background, NULL);

    // Initialize platforms
    game->platform_count = 3;
    game->platforms = malloc(sizeof(Platform) * game->platform_count);
    if (!game->platforms) {
        printf("Error: Failed to allocate platforms\n");
        game->state = 1;
        return 0;
    }

    // Platform 1: Center
    game->platforms[0].rect.x = (WIDTH - 200) / 2;
    game->platforms[0].rect.y = HEIGHT / 2;
    game->platforms[0].rect.w = 200;
    game->platforms[0].rect.h = 20;
    game->platforms[0].h_rect.x = game->platforms[0].rect.x + (game->platforms[0].rect.w / 18);
    game->platforms[0].h_rect.y = game->platforms[0].rect.y + (game->platforms[0].rect.h / 4);
    game->platforms[0].h_rect.w = game->platforms[0].rect.w - (game->platforms[0].rect.w / 8);
    game->platforms[0].h_rect.h = game->platforms[0].rect.h - (game->platforms[0].rect.h * 23 / 48);

    // Platform 2: Left
    game->platforms[1].rect.x = (WIDTH - 200) / 4;
    game->platforms[1].rect.y = HEIGHT / 3;
    game->platforms[1].rect.w = 200;
    game->platforms[1].rect.h = 20;
    game->platforms[1].h_rect.x = game->platforms[1].rect.x + (game->platforms[1].rect.w / 18);
    game->platforms[1].h_rect.y = game->platforms[1].rect.y + (game->platforms[1].rect.h / 4);
    game->platforms[1].h_rect.w = game->platforms[1].rect.w - (game->platforms[1].rect.w / 8);
    game->platforms[1].h_rect.h = game->platforms[1].rect.h - (game->platforms[1].rect.h * 23 / 48);

    // Platform 3: Right
    game->platforms[2].rect.x = 3 * (WIDTH - 200) / 4;
    game->platforms[2].rect.y = HEIGHT / 3;
    game->platforms[2].rect.w = 200;
    game->platforms[2].rect.h = 20;
    game->platforms[2].h_rect.x = game->platforms[2].rect.x + (game->platforms[2].rect.w / 18);
    game->platforms[2].h_rect.y = game->platforms[2].rect.y + (game->platforms[2].rect.h / 4);
    game->platforms[2].h_rect.w = game->platforms[2].rect.w - (game->platforms[2].rect.w / 8);
    game->platforms[2].h_rect.h = game->platforms[2].rect.h - (game->platforms[2].rect.h * 23 / 48);

    for (int i = 0; i < game->platform_count; i++) {
        printf("Platform %d initialized: rect=(x=%d, y=%d, w=%d, h=%d), h_rect=(x=%d, y=%d, w=%d, h=%d)\n",
               i, game->platforms[i].rect.x, game->platforms[i].rect.y, game->platforms[i].rect.w, game->platforms[i].rect.h,
               game->platforms[i].h_rect.x, game->platforms[i].h_rect.y, game->platforms[i].h_rect.w, game->platforms[i].h_rect.h);
    }

    // Initialize player2 if multiplayer
    if (game->multiplayer && !game->player2) {
        game->player2 = malloc(sizeof(Player));
        if (!game->player2) {
            printf("Error: Failed to allocate player2\n");
            free(game->platforms);
            game->state = 1;
            return 0;
        }
        game->player2->player_num = 1; // Default to purple bird
        initPlayer(game->player2);
        game->player2->rect.x = (WIDTH - SQUARE_SIZE) / 4; // Different starting position
        game->player2->h_rect.x = game->player2->rect.x + (SQUARE_SIZE / 1000);
        printf("Player2 initialized: player_num=%d, rect.x=%d\n", game->player2->player_num, game->player2->rect.x);
    }

    while (game->state == 0 && !game->quite) {
        Uint32 frame_start = SDL_GetTicks();

        if (game->background) {
            SDL_BlitSurface(game->background, NULL, game->screen, NULL);
        } else {
            SDL_FillRect(game->screen, NULL, SDL_MapRGB(game->screen->format, 0, 0, 0));
        }

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) game->quite = 1;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                game->state = 1;
            }
            handlePlayerMovement(game, game->player, e);
            if (game->player2 && game->multiplayer) {
                handlePlayer2Movement(game, game->player2, e);
            }
        }

        updatePlayer(game, game->player);
        if (game->player2 && game->multiplayer) {
            updatePlayer(game, game->player2);
        }

        for (int i = 0; i < game->platform_count; i++) {
            SDL_FillRect(game->screen, &game->platforms[i].rect, SDL_MapRGB(game->screen->format, 255, 255, 255));
        }

        renderPlayer(game->screen, game->player);
        if (game->player2 && game->multiplayer) {
            renderPlayer(game->screen, game->player2);
        }

        SDL_Flip(game->screen);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 1000 / game->fps) {
            SDL_Delay(1000 / game->fps - frame_time);
        }
    }

    return 0;
}

int gameplay2(Game *game) {
    if (!game || !game->screen || !game->player) {
        printf("Error: Invalid game, screen, or player pointer in gameplay2\n");
        game->state = 1;
        return 0;
    }

    // Load side-scrolling background
    SDL_Surface *scrolling_bg = IMG_Load(SIDE_SCROLLING_BG);
    if (!scrolling_bg) {
        printf("Error: Failed to load side-scrolling background: %s\n", IMG_GetError());
        game->state = 1;
        return 0;
    }

    // Store background dimensions
    int bg_width = scrolling_bg->w;
    int bg_height = scrolling_bg->h;
    int screen_width = game->width;
    int screen_height = game->height;

    // Scale background to fit screen height if necessary
    if (bg_height > screen_height) {
        SDL_Surface *temp = scrolling_bg;
        scrolling_bg = scaleSurface(temp, bg_width * screen_height / bg_height, screen_height);
        SDL_FreeSurface(temp);
        if (!scrolling_bg) {
            printf("Error: Failed to scale side-scrolling background\n");
            game->state = 1;
            return 0;
        }
        bg_width = bg_width * screen_height / bg_height;
        bg_height = screen_height;
    }

    // Initialize platforms (relative to background)
    game->platform_count = 3;
    if (game->platforms) {
        free(game->platforms);
        game->platforms = NULL;
    }
    game->platforms = malloc(sizeof(Platform) * game->platform_count);
    if (!game->platforms) {
        printf("Error: Failed to allocate platforms in gameplay2\n");
        SDL_FreeSurface(scrolling_bg);
        game->state = 1;
        return 0;
    }

    // Platform 1: Near start
    game->platforms[0].rect.x = bg_width / 8;
    game->platforms[0].rect.y = screen_height * 2 / 3;
    game->platforms[0].rect.w = 200;
    game->platforms[0].rect.h = 20;
    game->platforms[0].h_rect.x = game->platforms[0].rect.x + (game->platforms[0].rect.w / 18);
    game->platforms[0].h_rect.y = game->platforms[0].rect.y + (game->platforms[0].rect.h / 4);
    game->platforms[0].h_rect.w = game->platforms[0].rect.w - (game->platforms[0].rect.w / 8);
    game->platforms[0].h_rect.h = game->platforms[0].rect.h - (game->platforms[0].rect.h * 23 / 48);

    // Platform 2: Middle
    game->platforms[1].rect.x = bg_width / 2 - 100;
    game->platforms[1].rect.y = screen_height / 2;
    game->platforms[1].rect.w = 200;
    game->platforms[1].rect.h = 20;
    game->platforms[1].h_rect.x = game->platforms[1].rect.x + (game->platforms[1].rect.w / 18);
    game->platforms[1].h_rect.y = game->platforms[1].rect.y + (game->platforms[1].rect.h / 4);
    game->platforms[1].h_rect.w = game->platforms[1].rect.w - (game->platforms[1].rect.w / 8);
    game->platforms[1].h_rect.h = game->platforms[1].rect.h - (game->platforms[1].rect.h * 23 / 48);

    // Platform 3: Near end
    game->platforms[2].rect.x = 7 * bg_width / 8 - 100;
    game->platforms[2].rect.y = screen_height * 2 / 3;
    game->platforms[2].rect.w = 200;
    game->platforms[2].rect.h = 20;
    game->platforms[2].h_rect.x = game->platforms[2].rect.x + (game->platforms[2].rect.w / 18);
    game->platforms[2].h_rect.y = game->platforms[2].rect.y + (game->platforms[2].rect.h / 4);
    game->platforms[2].h_rect.w = game->platforms[2].rect.w - (game->platforms[2].rect.w / 8);
    game->platforms[2].h_rect.h = game->platforms[2].rect.h - (game->platforms[2].rect.h * 23 / 48);

    for (int i = 0; i < game->platform_count; i++) {
        printf("Platform %d initialized: rect=(x=%d, y=%d, w=%d, h=%d), h_rect=(x=%d, y=%d, w=%d, h=%d)\n",
               i, game->platforms[i].rect.x, game->platforms[i].rect.y, game->platforms[i].rect.w, game->platforms[i].rect.h,
               game->platforms[i].h_rect.x, game->platforms[i].h_rect.y, game->platforms[i].h_rect.w, game->platforms[i].h_rect.h);
    }

    // Initialize player2 if multiplayer
    if (game->multiplayer && !game->player2) {
        game->player2 = malloc(sizeof(Player));
        if (!game->player2) {
            printf("Error: Failed to allocate player2 in gameplay2\n");
            free(game->platforms);
            SDL_FreeSurface(scrolling_bg);
            game->state = 1;
            return 0;
        }
        game->player2->player_num = 1;
        initPlayer(game->player2);
        game->player2->x = bg_width / 8 + 200;
        game->player2->rect.x = game->player2->x;
        game->player2->h_rect.x = game->player2->x + (SQUARE_SIZE / 1000);
        printf("Player2 initialized: player_num=%d, x=%d\n", game->player2->player_num, game->player2->x);
    }

    // Reset player position
    game->player->x = bg_width / 8;
    game->player->rect.x = game->player->x;
    game->player->h_rect.x = game->player->x + (SQUARE_SIZE / 1000);
    game->player->y = screen_height - SQUARE_SIZE - 100;
    game->player->rect.y = game->player->y;
    game->player->h_rect.y = game->player->y + (SQUARE_SIZE / 3);

    // Initialize camera
    int camera_x = 0;

    while (game->state == 0 && !game->quite) {
        Uint32 frame_start = SDL_GetTicks();

        // Clear screen
        SDL_FillRect(game->screen, NULL, SDL_MapRGB(game->screen->format, 0, 0, 0));

        // Handle events
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) game->quite = 1;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                game->state = 1;
            }
            handlePlayerMovement(game, game->player, e);
            if (game->player2 && game->multiplayer) {
                handlePlayer2Movement(game, game->player2, e);
            }
        }

        // Update player
        updatePlayer(game, game->player);
        if (game->player2 && game->multiplayer) {
            updatePlayer(game, game->player2);
        }

        // Synchronize player->x and player->rect.x
        game->player->x = game->player->rect.x;
        game->player->y = game->player->rect.y;
        if (game->player2 && game->multiplayer) {
            game->player2->x = game->player2->rect.x;
            game->player2->y = game->player2->rect.y;
        }

        // Clamp player position to background boundaries
        if (game->player->x < 0) {
            game->player->x = 0;
            game->player->rect.x = 0;
            game->player->h_rect.x = game->player->x + (SQUARE_SIZE / 1000);
            game->player->x_speed = 0;
        }
        if (game->player->x > bg_width - SQUARE_SIZE) {
            game->player->x = bg_width - SQUARE_SIZE;
            game->player->rect.x = bg_width - SQUARE_SIZE;
            game->player->h_rect.x = game->player->x + (SQUARE_SIZE / 1000);
            game->player->x_speed = 0;
        }
        if (game->player->y < 0) {
            game->player->y = 0;
            game->player->rect.y = 0;
            game->player->h_rect.y = game->player->y + (SQUARE_SIZE / 3);
            game->player->y_speed = 0;
        }
        if (game->player->y > screen_height - SQUARE_SIZE) {
            game->player->y = screen_height - SQUARE_SIZE;
            game->player->rect.y = screen_height - SQUARE_SIZE;
            game->player->h_rect.y = game->player->y + (SQUARE_SIZE / 3);
            game->player->y_speed = 0;
        }

        // Clamp player2 position
        if (game->player2 && game->multiplayer) {
            if (game->player2->x < 0) {
                game->player2->x = 0;
                game->player2->rect.x = 0;
                game->player2->h_rect.x = game->player2->x + (SQUARE_SIZE / 1000);
                game->player2->x_speed = 0;
            }
            if (game->player2->x > bg_width - SQUARE_SIZE) {
                game->player2->x = bg_width - SQUARE_SIZE;
                game->player2->rect.x = bg_width - SQUARE_SIZE;
                game->player2->h_rect.x = game->player2->x + (SQUARE_SIZE / 1000);
                game->player2->x_speed = 0;
            }
            if (game->player2->y < 0) {
                game->player2->y = 0;
                game->player2->rect.y = 0;
                game->player2->h_rect.y = game->player2->y + (SQUARE_SIZE / 3);
                game->player2->y_speed = 0;
            }
            if (game->player2->y > screen_height - SQUARE_SIZE) {
                game->player2->y = screen_height - SQUARE_SIZE;
                game->player2->rect.y = screen_height - SQUARE_SIZE;
                game->player2->h_rect.y = game->player2->y + (SQUARE_SIZE / 3);
                game->player2->y_speed = 0;
            }
        }

        // Update camera (currently follows player1)
        camera_x = update_camera(game->player->x, screen_width, bg_width, SQUARE_SIZE);

        // Render background
        SDL_Rect src_rect = {camera_x, 0, screen_width, screen_height};
        SDL_Rect dst_rect = {0, 0, screen_width, screen_height};
        SDL_BlitSurface(scrolling_bg, &src_rect, game->screen, &dst_rect);

        // Render platforms (adjusted for camera offset)
        for (int i = 0; i < game->platform_count; i++) {
            SDL_Rect platform_rect = game->platforms[i].rect;
            platform_rect.x -= camera_x;
            SDL_FillRect(game->screen, &platform_rect, SDL_MapRGB(game->screen->format, 255, 255, 255));
        }

        // Render players (adjusted for camera offset)
        SDL_Rect player_rect = game->player->rect;
        player_rect.x = game->player->x - camera_x;
        SDL_BlitSurface(game->player->surface, NULL, game->screen, &player_rect);
        if (game->player2 && game->multiplayer) {
            SDL_Rect player2_rect = game->player2->rect;
            player2_rect.x = game->player2->x - camera_x;
            SDL_BlitSurface(game->player2->surface, NULL, game->screen, &player2_rect);
        }

        // Display player positions and camera info
        char pos_text[128];
        snprintf(pos_text, sizeof(pos_text), "P1: x=%.0f, y=%.0f | Camera: x=%.0f | BG: w=%d",
                 (float)game->player->x, (float)game->player->y, (float)camera_x, bg_width);
        Text *pos_display = safe_create_txt(pos_text, game->main_font, WHITE, 10, 10);
        if (pos_display && pos_display->surf) {
            SDL_BlitSurface(pos_display->surf, NULL, game->screen, &pos_display->rect);
            SDL_FreeSurface(pos_display->surf);
            free(pos_display->writen);
            free(pos_display);
        }

        // Update screen
        SDL_Flip(game->screen);

        // Control frame rate
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 1000 / game->fps) {
            SDL_Delay(1000 / game->fps - frame_time);
        }
    }

    // Cleanup
    SDL_FreeSurface(scrolling_bg);
    return 0;
}

int pizza() { return 69; }