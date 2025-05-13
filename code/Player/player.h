#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <stdbool.h>
#include "../game/game.h"

void initPlayer(Player *player);
void handlePlayerMovement(Player *player, SDL_Event e);
void checkPlatformCollision(Player *player, Platform *platform);
void updatePlayer(Player *player, Platform *platform);
void renderPlayer(SDL_Surface *screen, Player *player);
void freePlayer(Player *player);

#endif // PLAYER_H