//
// Created by lain on 4/19/25.
//


#include "time.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <time.h>
#include <stdio.h>
#include "../text/txt.h"

// Function to update FPS and frame count
void update_fps(Game *game) {
    static Uint32 cycle_start_time = 0; // Start time of current cycle
    Uint32 current_time = SDL_GetTicks();

    // Update FPS
    game->frame_count++;
    Uint32 elapsed_time = current_time - game->last_fps_update;
    if (elapsed_time >= 1000) {
        game->current_fps = (float)game->frame_count * 1000.0f / elapsed_time;
        game->frame_count = 0;
        game->last_fps_update = current_time;
    }

    // Update current_frame (cycles 1 to 120 in 500ms)
    if (cycle_start_time == 0) cycle_start_time = current_time;
    float elapsed_ms = (float)(current_time - cycle_start_time);
    const float cycle_duration_ms = 500.0f; // 500ms for 120 frames at 240 FPS
    const int total_frames = 120; // Cycle from 1 to 120
    float cycle_progress = elapsed_ms / cycle_duration_ms; // 0.0 to 1.0 over 500ms
    game->current_frame = (int)(cycle_progress * total_frames) % total_frames + 1;
    if (elapsed_ms >= cycle_duration_ms) cycle_start_time = current_time; // Reset cycle
}

// Function to update time string (HH:MM:SS)
void update_time(Game *game) {
    time_t current_time = time(NULL);
    if (current_time != game->last_time_update) {
        struct tm *tm_info = localtime(&current_time);
        strftime(game->time_string, sizeof(game->time_string), "Time: %H:%M:%S", tm_info);
        game->last_time_update = current_time;
    }
}

// Function to create a Text object
Text* create_time_text(Game *game, const char *text_str, TTF_Font *font, SDL_Color color, int x, int y) {
    Text *text = create_txt((char *)text_str, font, color, x, y); // Cast for const warning
    if (!text || !text->surf) {
        printf("Failed to create text: %s\n", TTF_GetError());
        return NULL;
    }
    return text;
}

// Function to render a Text object
void render_text(Game *game, Text *text) {
    if (!text || !text->surf || !game->screen) {
        printf("Invalid text or game state\n");
        return;
    }
    render_txt(game, text);
}

// Function to update square position based on mouse events
void update_square(Game *game, SDL_Rect *square_rect, int *dragging, int *offset_x, int *offset_y) {
    SDL_GetMouseState(&game->x_mouse, &game->y_mouse);
    while (SDL_PollEvent(&game->event)) {
        switch (game->event.type) {
            case SDL_QUIT:
                game->quite = 1;
                break;
            case SDL_KEYDOWN:
                if (game->event.key.keysym.sym == SDLK_ESCAPE) {
                    game->quite = 1;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (game->event.button.button == SDL_BUTTON_LEFT) {
                    if (game->x_mouse >= square_rect->x && game->x_mouse < square_rect->x + square_rect->w &&
                        game->y_mouse >= square_rect->y && game->y_mouse < square_rect->y + square_rect->h) {
                        *dragging = 1;
                        *offset_x = game->x_mouse - square_rect->x;
                        *offset_y = game->y_mouse - square_rect->y;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (game->event.button.button == SDL_BUTTON_LEFT) {
                    *dragging = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                if (*dragging) {
                    square_rect->x = game->x_mouse - *offset_x;
                    square_rect->y = game->y_mouse - *offset_y;
                    if (square_rect->x < 0) square_rect->x = 0;
                    if (square_rect->y < 0) square_rect->y = 0;
                    if (square_rect->x + square_rect->w > game->width) square_rect->x = game->width - square_rect->w;
                    if (square_rect->y + square_rect->h > game->height) square_rect->y = game->height - square_rect->h;
                }
                break;
        }
    }
}

// Function to render the scene (black screen, sprite, FPS/time/frame text)
void render_scene(Game *game, SDL_Surface *sprite, SDL_Rect *sprite_rect, Text *fps_text, Text *time_text, Text *frame_text, char *fps_string, char *frame_string) {
    // Update FPS text
    if (fps_text->surf) {
        SDL_FreeSurface(fps_text->surf);
        fps_text->surf = NULL;
    }
    snprintf(fps_string, 32, "FPS: %.1f", game->current_fps);
    Text *temp_fps_text = create_time_text(game, fps_string, game->main_font, (SDL_Color){255, 255, 255}, 20, 20);
    if (temp_fps_text) {
        *fps_text = *temp_fps_text;
        free(temp_fps_text);
    }

    // Update time text
    if (time_text->surf) {
        SDL_FreeSurface(time_text->surf);
        time_text->surf = NULL;
    }
    Text *temp_time_text = create_time_text(game, game->time_string, game->main_font, (SDL_Color){255, 255, 255}, 20, 50);
    if (temp_time_text) {
        *time_text = *temp_time_text;
        free(temp_time_text);
    }

    // Update frame text (show frame and FPS)
    if (frame_text->surf) {
        SDL_FreeSurface(frame_text->surf);
        frame_text->surf = NULL;
    }
    snprintf(frame_string, 32, "Frame: %d/%d", game->current_frame, FPS);
    Text *temp_frame_text = create_time_text(game, frame_string, game->main_font, (SDL_Color){255, 255, 255}, 20, 80);
    if (temp_frame_text) {
        *frame_text = *temp_frame_text;
        free(temp_frame_text);
    }

    // Render
    SDL_FillRect(game->screen, NULL, SDL_MapRGB(game->screen->format, 0, 0, 0)); // Black screen
    SDL_BlitSurface(sprite, NULL, game->screen, sprite_rect); // Sprite
    render_text(game, fps_text); // FPS text
    render_text(game, time_text); // Time text
    render_text(game, frame_text); // Frame text
    SDL_Flip(game->screen);
}

// Function to control frame rate (target 240 FPS)
void control_frame_rate(Uint32 frame_start, Uint32 frame_duration) {
    Uint32 frame_time = SDL_GetTicks() - frame_start;
    if (frame_time < frame_duration) {
        SDL_Delay(frame_duration - frame_time);
    }
}


