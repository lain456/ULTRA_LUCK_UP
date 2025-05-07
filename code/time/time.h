//
// Created by lain on 4/19/25.
//
#ifndef TIME_H
#define TIME_H

#include "../game/game.h"

void update_fps(Game *game);
void update_time(Game *game);
Text* create_time_text(Game *game, const char *text_str, TTF_Font *font, SDL_Color color, int x, int y);
void render_text(Game *game, Text *text);
void update_square(Game *game, SDL_Rect *square_rect, int *dragging, int *offset_x, int *offset_y);
void render_scene(Game *game, SDL_Surface *sprite, SDL_Rect *sprite_rect, Text *fps_text, Text *time_text, Text *frame_text, char *fps_string, char *frame_string);
void control_frame_rate(Uint32 frame_start, Uint32 frame_duration);

#endif //TIME_H




