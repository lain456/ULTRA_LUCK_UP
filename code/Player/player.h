//
// Created by lain on 4/3/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <stdbool.h>
#include "../game/game.h"



//#define MOVE_SPEED 5









void initPlayer(Player *player);
void handlePlayerMovement(Player *player, SDL_Event e);
void updatePlayer(Player *player);
void renderPlayer(SDL_Surface *screen, Player *player);
//void animate_player(Game*game ,Player *player);


#endif // PLAYER_H