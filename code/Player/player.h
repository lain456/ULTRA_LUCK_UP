#ifndef PLAYER_H
#define PLAYER_H
#include <SDL/SDL.h>
#include "../game/game.h"

void initPlayer(Player *player);
void handlePlayerMovement(Game *game ,Player *player, SDL_Event e);
void handlePlayer2Movement( Game *game, Player *player, SDL_Event e);
void checkPlatformCollision(Player *player, Platform *platform);
void updatePlayer(Game *game, Player *player);
void renderPlayer(SDL_Surface *screen, Player *player);
void freePlayer(Player *player);

#endif // PLAYER_H