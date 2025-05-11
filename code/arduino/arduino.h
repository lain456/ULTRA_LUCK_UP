//
// Created by lain on 4/30/25.
//

#ifndef ARDUINO_H
#define ARDUINO_H
#include "../game/game.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int init_sdl(Game *game);

int init_serial(Game *game, const char *port);

void parse_serial_data(Game *game);

#endif //ARDUINO_H