#include "game.h"
#include "../tools/tools.h"
#include "../Player/player.h"
#include "../Background/background.h"
#include "init.h"

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
            handlePlayerMovement(game->player, e);
            if (game->player2 && game->multiplayer) {
                handlePlayer2Movement(game->player2, e);
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

void handlePlayer2Movement(Player *player, SDL_Event e) {
    if (!player) {
        printf("Error: Player pointer is null in handlePlayer2Movement\n");
        return;
    }
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: player->moveLeft = 1; break;
            case SDLK_RIGHT: player->moveRight = 1; break;
            case SDLK_UP:
                if (!player->jump) {
                    player->jump_trigger = 1;
                    printf("Player2 jump triggered\n");
                }
                player->jump = 1;
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_LEFT: player->moveLeft = 0; break;
            case SDLK_RIGHT: player->moveRight = 0; break;
            case SDLK_UP: player->jump = 0; break;
        }
    }
}

int pizza() { return 69; }

