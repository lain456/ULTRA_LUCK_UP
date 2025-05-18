#include "my_input.h"
#include "../tools/tools.h"
#include <string.h>

// Initialize surfaces for input states
static void init_input_surfaces(My_input *my_input, int w, int h, InputStyle *style) {
    my_input->not_hovered = create_color_surface(w, h, style->not_hovered_color.r, style->not_hovered_color.g, style->not_hovered_color.b);
    my_input->hovered = create_color_surface(w, h, style->hovered_color.r, style->hovered_color.g, style->hovered_color.b);
    my_input->active = create_color_surface(w, h, style->active_color.r, style->active_color.g, style->active_color.b);
    my_input->bg = my_input->not_hovered; // Default state
}

// Initialize text for input
static void init_input_text(Game *game, My_input *my_input, int x, int y, int w, int h) {
    if (!game->main_font) {
        printf("Error: game->main_font is NULL\n");
        my_input->txt.surf = NULL;
        my_input->txt.rect.x = x + w / 2;
        my_input->txt.rect.y = y + (h - 20) / 2;
        return;
    }
    my_input->txt.font = game->main_font;
    my_input->txt.color = BLACK;
    my_input->txt.writen = my_input->value;
    // Use a space as fallback to avoid zero-width error
    my_input->txt.surf = TTF_RenderText_Solid(game->main_font, " ", my_input->txt.color);
    if (my_input->txt.surf) {
        my_input->txt.rect.x = x + (w - my_input->txt.surf->w) / 2;
        my_input->txt.rect.y = y + (h - my_input->txt.surf->h) / 2;
        printf("Initialized input text: rect=(x=%d, y=%d, w=%d, h=%d)\n",
               my_input->txt.rect.x, my_input->txt.rect.y, my_input->txt.surf->w, my_input->txt.surf->h);
    } else {
        printf("Failed to render input text: %s\n", TTF_GetError());
        my_input->txt.surf = NULL;
        my_input->txt.rect.x = x + w / 2;
        my_input->txt.rect.y = y + (h - 20) / 2;
    }
}

void init_my_input(Game *game, My_input *my_input, int x, int y, int w, int h, InputStyle *style) {
    my_input->rect.x = x;
    my_input->rect.y = y;
    my_input->rect.w = w;
    my_input->rect.h = h;
    my_input->is_active = 0;
    my_input->is_hovered = 0;
    my_input->cursor_visible = 0;
    my_input->last_cursor_toggle = SDL_GetTicks();
    my_input->max_length = 255;
    my_input->value[0] = '\0';

    init_input_surfaces(my_input, w, h, style);
    init_input_text(game, my_input, x, y, w, h);
}

// Update text surface when value changes
static void update_input_text(My_input *my_input) {
    if (!my_input->txt.font) {
        printf("Error: my_input->txt.font is NULL\n");
        return;
    }
    my_input->txt.writen = my_input->value;
    const char *text = (my_input->value[0] || my_input->is_active) ? my_input->value : " ";
    SDL_Surface *new_surf = TTF_RenderText_Solid(my_input->txt.font, text, my_input->txt.color);
    if (new_surf) {
        if (my_input->txt.surf) SDL_FreeSurface(my_input->txt.surf);
        my_input->txt.surf = new_surf;
        my_input->txt.rect.x = my_input->rect.x + (my_input->rect.w - my_input->txt.surf->w) / 2;
        my_input->txt.rect.y = my_input->rect.y + (my_input->rect.h - my_input->txt.surf->h) / 2;
        printf("Updated input text: '%s', rect=(x=%d, y=%d, w=%d, h=%d)\n",
               text, my_input->txt.rect.x, my_input->txt.rect.y, my_input->txt.surf->w, my_input->txt.surf->h);
    } else {
        printf("Failed to update input text: %s\n", TTF_GetError());
    }
}

// Update game->player_name with input value
static void sync_player_name(Game *game, My_input *my_input) {
    strncpy(game->player_name, my_input->value, sizeof(game->player_name) - 1);
    game->player_name[sizeof(game->player_name) - 1] = '\0';
    printf("Synced player_name: '%s'\n", game->player_name);
}

void handle_my_input_event(Game *game, My_input *my_input, SDL_Event *event) {
    static int cursor_pos = 0;
    switch (event->type) {
        case SDL_MOUSEMOTION:
            my_input->is_hovered = (event->motion.x >= my_input->rect.x &&
                                    event->motion.x <= my_input->rect.x + my_input->rect.w &&
                                    event->motion.y >= my_input->rect.y &&
                                    event->motion.y <= my_input->rect.y + my_input->rect.h);
            my_input->bg = my_input->is_hovered ? my_input->hovered : my_input->not_hovered;
            printf("Mouse motion: is_hovered=%d\n", my_input->is_hovered);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT && my_input->is_hovered) {
                my_input->is_active = 1;
                my_input->cursor_visible = 1;
                my_input->last_cursor_toggle = SDL_GetTicks();
                my_input->bg = my_input->active;
                cursor_pos = strlen(my_input->value);
                sync_player_name(game, my_input);
                printf("Input activated: cursor_pos=%d\n", cursor_pos);
            } else {
                my_input->is_active = 0;
                my_input->cursor_visible = 0;
                my_input->bg = my_input->is_hovered ? my_input->hovered : my_input->not_hovered;
                printf("Input deactivated\n");
            }
            update_input_text(my_input);
            break;
        case SDL_KEYDOWN:
            if (!my_input->is_active) break;
            printf("Key down: sym=%d, unicode=%d\n", event->key.keysym.sym, event->key.keysym.unicode);
            if (event->key.keysym.sym == SDLK_BACKSPACE && cursor_pos > 0) {
                my_input->value[cursor_pos - 1] = '\0';
                cursor_pos--;
                update_input_text(my_input);
                sync_player_name(game, my_input);
            } else if (event->key.keysym.sym == SDLK_RETURN) {
                my_input->is_active = 0;
                my_input->cursor_visible = 0;
                my_input->bg = my_input->is_hovered ? my_input->hovered : my_input->not_hovered;
                update_input_text(my_input);
                sync_player_name(game, my_input);
            } else if (event->key.keysym.unicode >= 32 && event->key.keysym.unicode <= 126 &&
                       cursor_pos < my_input->max_length) {
                my_input->value[cursor_pos] = (char)event->key.keysym.unicode;
                my_input->value[cursor_pos + 1] = '\0';
                cursor_pos++;
                update_input_text(my_input);
                sync_player_name(game, my_input);
            }
            break;
    }
}

void render_my_input(Game *game, My_input *my_input) {
    SDL_BlitSurface(my_input->bg, NULL, game->screen, &my_input->rect);
    if (my_input->txt.surf) {
        SDL_BlitSurface(my_input->txt.surf, NULL, game->screen, &my_input->txt.rect);
    }
    if (my_input->is_active) {
        Uint32 current_time = SDL_GetTicks();
        if (current_time - my_input->last_cursor_toggle >= 500) {
            my_input->cursor_visible = !my_input->cursor_visible;
            my_input->last_cursor_toggle = current_time;
        }
        if (my_input->cursor_visible) {
            int cursor_x = my_input->txt.surf ? my_input->txt.rect.x + my_input->txt.surf->w + 2 : my_input->rect.x + my_input->rect.w / 2;
            int cursor_h = my_input->txt.surf ? my_input->txt.surf->h : 20;
            SDL_Rect cursor_rect = {cursor_x, my_input->txt.rect.y, 2, cursor_h};
            SDL_FillRect(game->screen, &cursor_rect, SDL_MapRGB(game->screen->format, 0, 0, 0));
        }
    }
}

void render_my_inputs(Game *game, My_input *my_input, int i_ct) {
    for (int i = 0; i < i_ct; i++) {
        render_my_input(game, &my_input[i]);
    }
}