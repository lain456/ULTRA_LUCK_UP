#include "game.h"
#include "../tools/tools.h"
#include "../Player/player.h"

int gameplay(Game *game) {
    if (!game || !game->screen) {
        printf("Error: Invalid game or screen pointer\n");
        game->state = 1;
        return 0;
    }

    // Initialize player
    game->player = malloc(sizeof(Player));
    if (!game->player) {
        printf("Error: Failed to allocate player\n");
        game->state = 1;
        return 0;
    }
    memset(game->player, 0, sizeof(Player));
    initPlayer(game->player);
    printf("Player initialized\n");

    // Initialize platform
    Platform platform;
    platform.rect.x = (WIDTH - 200) / 2;
    platform.rect.y = HEIGHT / 2;
    platform.rect.w = 200;
    platform.rect.h = 20;
    // Hitbox: smaller and offset like buttons
    platform.h_rect.x = platform.rect.x + (platform.rect.w / 18);
    platform.h_rect.y = platform.rect.y + (platform.rect.h / 4);
    platform.h_rect.w = platform.rect.w - (platform.rect.w / 8);
    platform.h_rect.h = platform.rect.h - (platform.rect.h * 23 / 48);
    printf("Platform initialized: rect=(x=%d, y=%d, w=%d, h=%d), h_rect=(x=%d, y=%d, w=%d, h=%d)\n",
           platform.rect.x, platform.rect.y, platform.rect.w, platform.rect.h,
           platform.h_rect.x, platform.h_rect.y, platform.h_rect.w, platform.h_rect.h);

    while (game->state == 0 && !game->quite) {
        Uint32 frame_start = SDL_GetTicks();

        // Render background
        if (game->background) {
            SDL_BlitSurface(game->background, NULL, game->screen, NULL);
        } else {
            SDL_FillRect(game->screen, NULL, SDL_MapRGB(game->screen->format, 0, 0, 0));
        }

        // Handle input
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) game->quite = 1;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) game->state = 1;
            handlePlayerMovement(game->player, e);
        }

        // Update and render
        updatePlayer(game->player, &platform);
        SDL_FillRect(game->screen, &platform.rect, SDL_MapRGB(game->screen->format, 255, 255, 255));
        // Debug: Uncomment to visualize platform hitbox (red)
        // SDL_FillRect(game->screen, &platform.h_rect, SDL_MapRGB(screen->format, 255, 0, 0));
        renderPlayer(game->screen, game->player);
        SDL_Flip(game->screen);

        // FPS control
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 1000 / game->fps) {
            SDL_Delay(1000 / game->fps - frame_time);
        }
    }

    // Cleanup player
    if (game->player) {
        freePlayer(game->player);
        free(game->player);
        game->player = NULL;
    }

    return 0;
}

int pizza() { return 69; }