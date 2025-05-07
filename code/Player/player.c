//
// Created by lain on 4/3/25.
//
#include "player.h"
#include "../tools/tools.h"


void initPlayer(Player *player) {
    player->x = (WIDTH - SQUARE_SIZE) / 2;
    player->y = (HEIGHT - SQUARE_SIZE) / 2;
    player->index =0;
    player->cycle = 7;
    player->x_speed = 0;
    player->y_speed = 0.5;
    player->x_accel = 0;
    player->y_accel = 0;// gravity
    player->health = 10;
    player->level = 1; // Initialize level
    player->moveLeft = player->moveRight = player->moveUp = player->moveDown = false;
    //player->flying_to_the_left[0] = (SDL_Surface**) malloc(sizeof(SDL_Surface*)*8);



    player->flying_to_the_right[0] = scaleSurface(IMG_Load(BIRD_R01_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_right[1] = scaleSurface(IMG_Load(BIRD_R02_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_right[2] = scaleSurface(IMG_Load(BIRD_R03_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_right[3] = scaleSurface(IMG_Load(BIRD_R04_PATH ),SQUARE_SIZE, SQUARE_SIZE);





    player->flying_to_the_left[0] = scaleSurface(IMG_Load(BIRD_L01_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_left[1] = scaleSurface(IMG_Load(BIRD_L02_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_left[2] = scaleSurface(IMG_Load(BIRD_L03_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_left[3] = scaleSurface(IMG_Load(BIRD_L04_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_left[4] = scaleSurface(IMG_Load(BIRD_L05_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_left[5] = scaleSurface(IMG_Load(BIRD_L06_PATH ),SQUARE_SIZE, SQUARE_SIZE);
    player->flying_to_the_left[6] = scaleSurface(IMG_Load(BIRD_L07_PATH ),SQUARE_SIZE, SQUARE_SIZE);

























    // keys :
    player->input.right = SDLK_d;
    player->input.left = SDLK_q;
    player->input.up = SDLK_z;
    player->input.down = SDLK_s;
    player->input.dash = SDLK_SPACE;







    // LET'S GOOOO IT WORKS MF AHAHAHAAHAH












    player->surface = player->flying_to_the_right[0];
    //SDL_FillRect(player->surface, NULL, SDL_MapRGB(player->surface->format, 255, 0, 0));
}

void handlePlayerMovement(Player *player, SDL_Event e) {
    const int right  = (int) player->input.right;



    player->moveDown =0;
    player->moveRight =0;
    player->moveLeft =0;
    player->moveUp =0;
    bool isKeyDown = (e.type == SDL_KEYDOWN);
    switch (e.key.keysym.sym) {
    case SDLK_q: player->moveLeft = isKeyDown; break;
    case SDLK_z: player->moveUp = isKeyDown; break;
    case SDLK_s: player->moveDown = isKeyDown; break;
    case SDLK_d : player->moveRight = isKeyDown; break;
    }
    while (SDL_PollEvent(&e))
    {
        if (e.key.keysym.sym == right )
        {
            player->moveRight = isKeyDown;
        }
    }

}

void updatePlayer(Player *player) {
    // static state
    player->x_speed = 0;
    player->y_speed = 0;
    if (player->moveLeft) player->x_speed = - MOVE_SPEED ;
    if (player->moveRight) player->x_speed = MOVE_SPEED;
    if (player->moveUp){
        //player->y_speed -= player->y_accel; // fly
        player->y_speed = - MOVE_SPEED; // jump
    }
    if (player->moveDown) { player->y_speed =  MOVE_SPEED; }
    if (!player->moveUp) {
        player->y_speed += player->y_accel;
    }

    // Update player position
    player->x += player->x_speed;
    player->y += player->y_speed;

    // speed limit (cuz the phisics kinda suck)
    if (player-> y_speed > 7) player->y_speed = 7;
    if (player-> y_speed < -7) player->y_speed = -7;
}











void renderPlayer(SDL_Surface *screen, Player *player) {
    // for now it will just draw a red square but we will change it later
    SDL_Rect rect = { player->x, player->y, SQUARE_SIZE, SQUARE_SIZE };
    SDL_BlitSurface(player->surface, NULL, screen, &rect);
}











































