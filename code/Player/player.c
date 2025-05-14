#include "player.h"
#include "../tools/tools.h"
#include "../game/game.h"
#include <SDL/SDL_image.h>

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
    player->jump_count = 0;
    player->max_jumps = 2;
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

    // Set initial surface based on player_num using switch
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















void handlePlayerMovement(Player *player, SDL_Event e) {
    if (!player) {
        printf("Error: Player pointer is null in handlePlayerMovement\n");
        return;
    }
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_q: player->moveLeft = 1; break;
            case SDLK_d: player->moveRight = 1; break;
            case SDLK_SPACE:
                if (!player->jump) { // Only trigger if not already jumping
                    player->jump_trigger = 1;
                    printf("Jump triggered\n");
                }
                player->jump = 1;
                break;
        }
    } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_q: player->moveLeft = 0; break;
            case SDLK_d: player->moveRight = 0; break;
            case SDLK_SPACE: player->jump = 0; break;
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
        // Landing from above
        if (player->y_speed >= 0 && player->h_rect.y + player->h_rect.h <= platform->h_rect.y + 10) {
            player->rect.y = platform->h_rect.y - player->h_rect.h - (player->h_rect.y - player->rect.y);
            player->h_rect.y = platform->h_rect.y - player->h_rect.h;
            player->y_speed = 0;
            player->y_accel = 0;
            player->jump_count = 0;
            printf("Landed on platform, jump_count reset to %d, rect.y=%d, h_rect.y=%d\n",
                   player->jump_count, player->rect.y, player->h_rect.y);
        }
    } else {
        player->y_accel = 0.2; // Lighter gravity
    }
}

void updatePlayer(Player *player, Platform *platform) {
    if (!player) {
        printf("Error: Player pointer is null in updatePlayer\n");
        return;
    }

    static int is_grounded = 0;
    static int frame_counter = 0; // For animation timing

    is_grounded = (player->rect.y >= HEIGHT - SQUARE_SIZE && player->y_speed == 0) ||
                  (player->y_accel == 0 && player->y_speed == 0);

    // Handle jump
    if (player->jump_trigger && player->jump_count < player->max_jumps) {
        player->y_speed = -12; // Stronger jump
        player->y_accel = 0.2; // Lighter gravity
        player->jump_count++;
        player->jump_trigger = 0; // Consume trigger
        is_grounded = 0;
        printf("Jump %d performed, jump_count=%d, y_speed=%.2f\n", player->jump_count, player->jump_count, player->y_speed);
    }

    // Reset jump count when grounded
    if (is_grounded) {
        player->jump_count = 0;
        printf("Grounded, jump_count reset to %d\n", player->jump_count);
    }

    // Horizontal movement
    float target_x_speed = 0;
    if (player->moveLeft) target_x_speed = -5.0;
    if (player->moveRight) target_x_speed = 5.0;
    player->x_speed += (target_x_speed - player->x_speed) * 0.2;
    if (target_x_speed == 0) player->x_speed *= 0.8;
    if (player->moveLeft) player->move = -1;
    if (player->moveRight) player->move = 1;

    // Apply gravity
    player->y_speed += player->y_accel;
    if (player->y_speed > 12) player->y_speed = 12; // Adjusted cap
    if (player->y_speed < -12) player->y_speed = -12;

    // Update position
    player->rect.x += player->x_speed;
    player->rect.y += player->y_speed;
    // Update hitbox position with offset
    player->h_rect.x = player->rect.x + (player->rect.w / 18);
    player->h_rect.y = player->rect.y + (player->rect.h / 4);

    // Keep on-screen
    if (player->rect.x < 0) {
        player->rect.x = 0;
        player->h_rect.x = player->rect.x + (player->rect.w / 18);
        player->x_speed = 0;
    }
    if (player->rect.x > WIDTH - SQUARE_SIZE) {
        player->rect.x = WIDTH - SQUARE_SIZE;
        player->h_rect.x = player->rect.x + (player->rect.w / 18);
        player->x_speed = 0;
    }
    if (player->rect.y < 0) {
        player->rect.y = 0;
        player->h_rect.y = player->rect.y + (player->rect.h / 4);
        player->y_speed = 0;
    }
    if (player->rect.y > HEIGHT - SQUARE_SIZE) {
        player->rect.y = HEIGHT - SQUARE_SIZE;
        player->h_rect.y = player->rect.y + (player->rect.h / 4);
        player->y_speed = 0;
        player->y_accel = 0;
        is_grounded = 1;
        player->jump_count = 0;
        printf("Landed on ground, jump_count reset to %d, rect.y=%d, h_rect.y=%d\n",
               player->jump_count, player->rect.y, player->h_rect.y);
    }

    // Check platform collision
    checkPlatformCollision(player, platform);

    // Update animation (every ~4 frames for ~15 FPS at 60 FPS)
    frame_counter++;
    if (frame_counter >= 4) {
        player->index = (player->index + 1) % player->cycle;
        frame_counter = 0;
    }

    // Set surface based on direction and player_num using switch
    if (player->index >= 0 && player->index < player->cycle) {
        SDL_Surface *new_surface = NULL;
        switch (player->player_num) {
            case 1: // Purple bird
                new_surface = (player->move == 1) ? player->p_flying_to_the_right[player->index] :
                              player->p_flying_to_the_left[player->index];
                break;
            case 0: // Yellow bird
            default: // Default to yellow for any unexpected player_num
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
        // Fallback: red rectangle
        SDL_FillRect(screen, &player->rect, SDL_MapRGB(screen->format, 255, 0, 0));
    }

    // Debug: Uncomment to visualize player hitbox (red)
    //SDL_FillRect(screen, &player->rect, SDL_MapRGB(screen->format, 0, 255, 0));
    //SDL_FillRect(screen, &player->h_rect, SDL_MapRGB(screen->format, 255, 0, 0));
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
    player->surface = NULL; // Surface already freed if part of animation arrays
}