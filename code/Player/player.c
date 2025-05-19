#include "player.h"
#include "../tools/tools.h"
#include "../game/game.h"
#include <SDL/SDL_image.h>
#include "../keymap/keymap.h"

void initPlayer(Player *player) {
    if (!player) {
        printf("Error: Player pointer is null in initPlayer\n");
        return;
    }

    printf("Starting player initialization\n");
    // Initialize fields
    player->rect.x = (WIDTH - SQUARE_SIZE) / 2;
    player->rect.y = HEIGHT - SQUARE_SIZE - 100;
    player->rect.w = SQUARE_SIZE;
    player->rect.h = SQUARE_SIZE;
    player->h_rect.x = player->rect.x + (SQUARE_SIZE / 1000);
    player->h_rect.y = player->rect.y + (SQUARE_SIZE / 3);
    player->h_rect.w = SQUARE_SIZE - (SQUARE_SIZE / 7);
    player->h_rect.h = SQUARE_SIZE - (SQUARE_SIZE * 20 / 48);
    player->x_speed = 0;
    player->y_speed = 0;
    player->y_accel = 0.2;
    player->moveLeft = 0;
    player->moveRight = 0;
    player->jump = 0;
    player->jump_trigger = 0;
    player->move = 1;
    player->surface = NULL;
    player->index = 0;
    player->cycle = 7;
    player->frame_counter = 0;
    player->jump_count = 0;
    player->max_jumps = 2;
    player->is_dashing = 0; // Initialize dash fields
    player->dash_start_time = 0;
    player->dash_cooldown_end = 0;
    for (int i = 0; i < 7; i++) {
        player->flying_to_the_right[i] = NULL;
        player->flying_to_the_left[i] = NULL;
        player->p_flying_to_the_right[i] = NULL;
        player->p_flying_to_the_left[i] = NULL;
    }

    printf("Player initialized: rect=(x=%d, y=%d, w=%d, h=%d), h_rect=(x=%d, y=%d, w=%d, h=%d), jump_count=%d\n",
           player->rect.x, player->rect.y, player->rect.w, player->rect.h,
           player->h_rect.x, player->h_rect.y, player->h_rect.w, player->h_rect.h, player->jump_count);

    // Load yellow bird animation sprites
    printf("Loading yellow bird animation sprites\n");
    SDL_Surface *temp = IMG_Load(BIRD_R01_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R01_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[0] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[0]) {
        printf("Error: Failed to scale %s\n", BIRD_R01_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_R02_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R02_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[1] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[1]) {
        printf("Error: Failed to scale %s\n", BIRD_R02_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_R03_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R03_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[2] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[2]) {
        printf("Error: Failed to scale %s\n", BIRD_R03_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_R04_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R04_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[3] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[3]) {
        printf("Error: Failed to scale %s\n", BIRD_R04_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_R05_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R05_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[4] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[4]) {
        printf("Error: Failed to scale %s\n", BIRD_R05_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_R06_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R06_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[5] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[5]) {
        printf("Error: Failed to scale %s\n", BIRD_R06_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_R07_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_R07_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_right[6] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_right[6]) {
        printf("Error: Failed to scale %s\n", BIRD_R07_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L01_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L01_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[0] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[0]) {
        printf("Error: Failed to scale %s\n", BIRD_L01_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L02_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L02_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[1] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[1]) {
        printf("Error: Failed to scale %s\n", BIRD_L02_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L03_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L03_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[2] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[2]) {
        printf("Error: Failed to scale %s\n", BIRD_L03_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L04_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L04_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[3] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[3]) {
        printf("Error: Failed to scale %s\n", BIRD_L04_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L05_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L05_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[4] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[4]) {
        printf("Error: Failed to scale %s\n", BIRD_L05_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L06_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L06_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[5] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[5]) {
        printf("Error: Failed to scale %s\n", BIRD_L06_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(BIRD_L07_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", BIRD_L07_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->flying_to_the_left[6] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->flying_to_the_left[6]) {
        printf("Error: Failed to scale %s\n", BIRD_L07_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    // Load purple bird animation sprites
    printf("Loading purple bird animation sprites\n");
    temp = IMG_Load(P_BIRD_R01_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R01_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[0] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[0]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R01_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_R02_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R02_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[1] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[1]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R02_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_R03_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R03_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[2] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[2]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R03_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_R04_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R04_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[3] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[3]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R04_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_R05_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R05_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[4] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[4]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R05_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_R06_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R06_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[5] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[5]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R06_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_R07_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_R07_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_right[6] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_right[6]) {
        printf("Error: Failed to scale %s\n", P_BIRD_R07_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L01_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L01_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[0] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[0]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L01_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L02_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L02_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[1] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[1]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L02_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L03_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L03_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[2] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[2]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L03_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L04_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L04_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[3] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[3]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L04_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L05_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L05_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[4] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[4]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L05_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L06_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L06_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[5] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[5]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L06_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    temp = IMG_Load(P_BIRD_L07_PATH);
    if (!temp) {
        printf("Error: Failed to load %s: %s\n", P_BIRD_L07_PATH, IMG_GetError());
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }
    player->p_flying_to_the_left[6] = scaleSurface(temp, SQUARE_SIZE, SQUARE_SIZE);
    SDL_FreeSurface(temp);
    if (!player->p_flying_to_the_left[6]) {
        printf("Error: Failed to scale %s\n", P_BIRD_L07_PATH);
        player->surface = NULL;
        printf("Sprite loading failed, using fallback red rectangle\n");
        return;
    }

    // Set initial surface based on player_num
    switch (player->player_num) {
        case 1: // Purple bird
            player->surface = player->p_flying_to_the_right[0];
            break;
        case 0: // Yellow bird
        default:
            player->surface = player->flying_to_the_right[0];
            break;
    }

    if (!player->surface) {
        printf("Error: No valid sprites loaded, using fallback red rectangle\n");
        return;
    }

    printf("Player sprite loaded successfully (player_num=%d)\n", player->player_num);
}

void handlePlayerMovement(Game *game, Player *player, SDL_Event e) {
    if (!player) {
        printf("Error: Player pointer is null in handlePlayerMovement\n");
        return;
    }

    const Uint32 JUMP = (Uint32)key_name_to_sdlkey(game->controls_p1.jump);
    const Uint32 left = key_name_to_sdlkey(game->controls_p1.left);
    const Uint32 right = key_name_to_sdlkey(game->controls_p1.right);
    const Uint32 down = key_name_to_sdlkey(game->controls_p1.down);
    const Uint32 up = key_name_to_sdlkey(game->controls_p1.up);
    const Uint32 dash = key_name_to_sdlkey(game->controls_p1.dash); // Added for dash

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == left) {
            player->moveLeft = 1;
        } else if (e.key.keysym.sym == right) {
            player->moveRight = 1;
        } else if (e.key.keysym.sym == JUMP) {
            if (!player->jump) {
                player->jump_trigger = 1;
                printf("Jump triggered\n");
            }
            player->jump = 1;
        } else if (e.key.keysym.sym == dash && !player->is_dashing) {
            // Check if dash is off cooldown
            if (SDL_GetTicks() >= player->dash_cooldown_end) {
                player->is_dashing = 1;
                player->dash_start_time = SDL_GetTicks();
                printf("Dash triggered\n");
            }
        }
    } else if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == left) {
            player->moveLeft = 0;
        } else if (e.key.keysym.sym == right) {
            player->moveRight = 0;
        } else if (e.key.keysym.sym == JUMP) {
            player->jump = 0;
        }
    }
}

void handlePlayer2Movement(Game *game, Player *player, SDL_Event e) {
    if (!player) {
        printf("Error: Player pointer is null in handlePlayer2Movement\n");
        return;
    }

    const Uint32 JUMP = (Uint32)key_name_to_sdlkey(game->controls_p2.jump);
    const Uint32 left = key_name_to_sdlkey(game->controls_p2.left);
    const Uint32 right = key_name_to_sdlkey(game->controls_p2.right);
    const Uint32 down = key_name_to_sdlkey(game->controls_p2.down);
    const Uint32 up = key_name_to_sdlkey(game->controls_p2.up);
    const Uint32 dash = key_name_to_sdlkey(game->controls_p2.dash); // Added for dash

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == left) {
            player->moveLeft = 1;
        } else if (e.key.keysym.sym == right) {
            player->moveRight = 1;
        } else if (e.key.keysym.sym == JUMP) {
            if (!player->jump) {
                player->jump_trigger = 1;
                printf("Jump triggered\n");
            }
            player->jump = 1;
        } else if (e.key.keysym.sym == dash && !player->is_dashing) {
            // Check if dash is off cooldown
            if (SDL_GetTicks() >= player->dash_cooldown_end) {
                player->is_dashing = 1;
                player->dash_start_time = SDL_GetTicks();
                printf("Dash triggered\n");
            }
        }
    } else if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == left) {
            player->moveLeft = 0;
        } else if (e.key.keysym.sym == right) {
            player->moveRight = 0;
        } else if (e.key.keysym.sym == JUMP) {
            player->jump = 0;
        }
    }
}

void checkPlatformCollision(Player *player, Platform *platform) {
    if (!player || !platform) {
        printf("Error: Null pointer in checkPlatformCollision\n");
        return;
    }

    int collision = (player->h_rect.x < platform->h_rect.x + platform->h_rect.w &&
                    player->h_rect.x + player->h_rect.w > platform->h_rect.x &&
                    player->h_rect.y < platform->h_rect.y + platform->h_rect.h &&
                    player->h_rect.y + player->h_rect.h > platform->h_rect.y);

    if (collision) {
        if (player->y_speed >= 0 && player->h_rect.y + player->h_rect.h <= platform->h_rect.y + 10) {
            player->rect.y = platform->h_rect.y - player->h_rect.h - (player->h_rect.y - player->rect.y);
            player->h_rect.y = platform->h_rect.y - player->h_rect.h;
            player->y_speed = 0;
            player->y_accel = 0;
            player->jump_count = 0;
            printf("Landed on platform, jump_count reset to %d, rect.y=%d, h_rect.y=%d\n",
                   player->jump_count, player->rect.y, player->h_rect.y);
        }
    }
}

void updatePlayer(Game *game, Player *player) {
    if (!game || !player) {
        printf("Error: Invalid game or player pointer in updatePlayer\n");
        return;
    }

    int is_grounded = (player->rect.y >= HEIGHT - SQUARE_SIZE && player->y_speed == 0) ||
                      (player->y_accel == 0 && player->y_speed == 0);

    // Handle jump
    if (player->jump_trigger && player->jump_count < player->max_jumps) {
        player->y_speed = -12;
        player->y_accel = 0.2;
        player->jump_count++;
        player->jump_trigger = 0;
        is_grounded = 0;
        printf("Jump %d performed, jump_count=%d, y_speed=%.2f\n", player->jump_count, player->jump_count, player->y_speed);
    }

    if (is_grounded) {
        player->jump_count = 0;
        printf("Grounded, jump_count reset to %d\n", player->jump_count);
    }

    // Handle dash
    const Uint32 DASH_DURATION = 200; // 200ms dash
    const Uint32 DASH_COOLDOWN = 1000; // 1s cooldown
    const float DASH_SPEED = 15.0; // Dash speed
    if (player->is_dashing) {
        Uint32 current_time = SDL_GetTicks();
        if (current_time - player->dash_start_time <= DASH_DURATION) {
            // Apply dash movement
            player->x_speed = (player->move == 1) ? DASH_SPEED : -DASH_SPEED;
            // Prevent normal movement inputs during dash
            player->moveLeft = 0;
            player->moveRight = 0;
        } else {
            // End dash
            player->is_dashing = 0;
            player->x_speed = 0; // Reset speed after dash
            player->dash_cooldown_end = current_time + DASH_COOLDOWN;
            printf("Dash ended, cooldown until %u\n", player->dash_cooldown_end);
        }
    } else {
        // Normal horizontal movement
        float target_x_speed = 0;
        if (player->moveLeft) target_x_speed = -5.0;
        if (player->moveRight) target_x_speed = 5.0;
        player->x_speed += (target_x_speed - player->x_speed) * 0.2;
        if (target_x_speed == 0) player->x_speed *= 0.8;
        if (player->moveLeft) player->move = -1;
        if (player->moveRight) player->move = 1;
    }

    // Gravity
    player->y_speed += player->y_accel;
    if (player->y_speed > 12) player->y_speed = 12;
    if (player->y_speed < -12) player->y_speed = -12;

    // Update position
    player->rect.x += player->x_speed;
    player->rect.y += player->y_speed;
    player->h_rect.x = player->rect.x + (player->rect.w / 18);
    player->h_rect.y = player->rect.y + (player->rect.h / 4);

    // Keep on-screen
    if (player->rect.x < 0) {
        player->rect.x = 0;
        player->h_rect.x = player->rect.x + (player->rect.w / 18);
        player->x_speed = 0;
    }
    if (player->rect.x > game->lvl_x_size - SQUARE_SIZE) {
        player->rect.x = game->lvl_x_size - SQUARE_SIZE;
        player->h_rect.x = player->rect.x + (player->rect.w / 18);
        player->x_speed = 0;
    }
    if (player->rect.y < 0) {
        player->rect.y = 0;
        player->h_rect.y = player->rect.y + (player->rect.h / 4);
        player->y_speed = 0;
    }
    if (player->rect.y > game->lvl_y_size - SQUARE_SIZE) {
        player->rect.y = game->lvl_y_size - SQUARE_SIZE;
        player->h_rect.y = player->rect.y + (player->rect.h / 4);
        player->y_speed = 0;
        player->y_accel = 0;
        is_grounded = 1;
        player->jump_count = 0;
        printf("Landed on ground, jump_count reset to %d, rect.y=%d, h_rect.y=%d\n",
               player->jump_count, player->rect.y, player->h_rect.y);
    }

    // Check platform collisions
    player->y_accel = 0.2;
    for (int i = 0; i < game->platform_count; i++) {
        checkPlatformCollision(player, &game->platforms[i]);
    }

    // Update animation
    int frame_delay = player->is_dashing ? 2 : 4; // Faster animation during dash
    player->frame_counter++;
    if (player->frame_counter >= frame_delay) {
        player->index = (player->index + 1) % player->cycle;
        player->frame_counter = 0;
    }

    // Set surface based on direction and player_num
    if (player->index >= 0 && player->index < player->cycle) {
        SDL_Surface *new_surface = NULL;
        switch (player->player_num) {
            case 1: // Purple bird
                new_surface = (player->move == 1) ? player->p_flying_to_the_right[player->index] :
                              player->p_flying_to_the_left[player->index];
                break;
            case 0: // Yellow bird
            default:
                new_surface = (player->move == 1) ? player->flying_to_the_right[player->index] :
                              player->flying_to_the_left[player->index];
                break;
        }
        if (new_surface) {
            player->surface = new_surface;
        } else {
            printf("Warning: Null sprite at index %d, direction %d, player_num %d\n",
                   player->index, player->move, player->player_num);
            switch (player->player_num) {
                case 1:
                    new_surface = player->p_flying_to_the_right[0];
                    break;
                case 0:
                default:
                    new_surface = player->flying_to_the_right[0];
                    break;
            }
            player->surface = new_surface;
        }
    } else {
        printf("Error: Invalid animation index %d, resetting to 0\n", player->index);
        player->index = 0;
        switch (player->player_num) {
            case 1:
                player->surface = player->p_flying_to_the_right[0];
                break;
            case 0:
            default:
                player->surface = player->flying_to_the_right[0];
                break;
        }
    }
}

void renderPlayer(SDL_Surface *screen, Player *player) {
    if (!screen || !player) {
        printf("Error: Invalid pointers in renderPlayer\n");
        return;
    }

    if (player->surface) {
        SDL_BlitSurface(player->surface, NULL, screen, &player->rect);
    } else {
        SDL_FillRect(screen, &player->rect, SDL_MapRGB(screen->format, 255, 0, 0));
    }
}

void freePlayer(Player *player) {
    if (!player) return;
    printf("Freeing player resources\n");
    for (int i = 0; i < 7; i++) {
        if (player->flying_to_the_right[i]) {
            SDL_FreeSurface(player->flying_to_the_right[i]);
            player->flying_to_the_right[i] = NULL;
        }
        if (player->flying_to_the_left[i]) {
            SDL_FreeSurface(player->flying_to_the_left[i]);
            player->flying_to_the_left[i] = NULL;
        }
        if (player->p_flying_to_the_right[i]) {
            SDL_FreeSurface(player->p_flying_to_the_right[i]);
            player->p_flying_to_the_right[i] = NULL;
        }
        if (player->p_flying_to_the_left[i]) {
            SDL_FreeSurface(player->p_flying_to_the_left[i]);
            player->p_flying_to_the_left[i] = NULL;
        }
    }
    player->surface = NULL;
}