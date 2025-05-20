#include "arduino.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int init_sdl(Game *game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init failed: %s\n", SDL_GetError());
        return 0;
    }

    game->screen = SDL_SetVideoMode(game->width, game->height, 32, SDL_SWSURFACE);
    if (!game->screen) {
        printf("SDL SetVideoMode failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    if (TTF_Init() < 0) {
        printf("TTF Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    game->main_font = TTF_OpenFont(FREDOKA_PATH, 24);
    if (!game->main_font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    return 1;
}

int init_serial(Game *game, const char *port) {
    game->serial_fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (game->serial_fd == -1) {
        printf("Failed to open serial port %s\n", port);
        game->controler_enabled = 0;
        return 0;
    }else
    {
        printf("Opened serial port %s\n", port);
        game->controler_enabled = 1;
    }

    struct termios options;
    tcgetattr(game->serial_fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(game->serial_fd, TCSANOW, &options);
    return 1;
}

void parse_serial_data(Game *game) {
    char buffer[256];
    int n = read(game->serial_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        strncat(game->serial_buffer, buffer, sizeof(game->serial_buffer) - game->serial_buffer_len - 1);
        game->serial_buffer_len += n;

        char *line = strtok(game->serial_buffer, "\n");
        while (line) {
            sscanf(line, "B9:%d B10:%d B11:%d B12:%d B13:%d JX:%d JY:%d JSW:%d Green:%s Red:%s",
                   &game->arduino_status.B9, &game->arduino_status.B10, &game->arduino_status.B11,
                   &game->arduino_status.B12, &game->arduino_status.B13, &game->arduino_status.JX,
                   &game->arduino_status.JY, &game->arduino_status.JSW,
                   game->arduino_status.Green, game->arduino_status.Red);
            game->serial_buffer_len = 0;
            game->serial_buffer[0] = '\0';
            line = strtok(NULL, "\n");
        }
    }



   // printf("Parsed JX: %d, JY: %d\n", game->arduino_status.JX, game->arduino_status.JY);
    printf("Arduino State: B9:%d B10:%d B11:%d B12:%d B13:%d JX:%d JY:%d JSW:%d Green:%s Red:%s\n",
               game->arduino_status.B9, game->arduino_status.B10, game->arduino_status.B11,
               game->arduino_status.B12, game->arduino_status.B13, game->arduino_status.JX,
               game->arduino_status.JY, game->arduino_status.JSW,
               game->arduino_status.Green, game->arduino_status.Red);
}