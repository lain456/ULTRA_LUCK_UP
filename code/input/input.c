//
// Created by lain on 5/15/25.
//

#include "input.h"
#include "../tools/tools.h"
#include "../text/txt.h"
#include "../button/button.h"

Input_box *create_input_box(Game *game, int x, int y, int w, int h, char *initial_text, SDL_Color text_color, int max_length) {

    Input_box *input_box = (Input_box *)malloc(sizeof(Input_box));
    if (!input_box) {
        printf("Error: Failed to allocate input_box\n");
        return NULL;
    }

    input_box->rect = (SDL_Rect){x, y, w, h};
    strncpy(input_box->value, initial_text, sizeof(input_box->value) - 1);
    input_box->value[sizeof(input_box->value) - 1] = '\0';
    input_box->max_length = max_length;
    input_box->is_active = 0;
    input_box->is_hovered = 0;

    // Create surfaces for different states
    input_box->not_hovered = scaleSurface(game->b_purple, w, h);
    input_box->hovered = scaleSurface(game->b_yellow, w, h);
    input_box->active = create_color_surface(w, h, 200, 200, 200); // Light gray when active

    // Initialize text, centered
    input_box->txt = *create_txt(initial_text, game->main_font, text_color, x, y);
    input_box->txt.rect.x = x + (w - input_box->txt.surf->w) / 2; // Center horizontally
    input_box->txt.rect.y = y + (h - input_box->txt.surf->h) / 2; // Center vertically

    return input_box;
}

void update_input_box(Game *game, Input_box *input_box) {
// Update hover state
input_box->is_hovered = is_hovered(game, &input_box->rect);

// Handle click to activate/deactivate
if (is_clicked(game, &input_box->rect)) {
input_box->is_active = 1;
} else if (game->mouse_pressed && !input_box->is_hovered) {
input_box->is_active = 0;
}

// Handle keyboard input when active
if (input_box->is_active && game->event.type == SDL_KEYDOWN) {
SDLKey key = game->event.key.keysym.sym;
if (key == SDLK_BACKSPACE && strlen(input_box->value) > 0) {
input_box->value[strlen(input_box->value) - 1] = '\0';
} else if (key >= SDLK_a && key <= SDLK_z && strlen(input_box->value) < input_box->max_length) {
char c = (char)(key - SDLK_a + 'a');
if (game->event.key.keysym.mod & KMOD_SHIFT) {
c = c - 32; // Uppercase
}
char temp[2] = {c, '\0'};
strncat(input_box->value, temp, input_box->max_length - strlen(input_box->value));
}
// Update text with new value, centered
update_txt(&input_box->txt, input_box->value[0] ? input_box->value : " ", input_box->txt.color, NULL);
}
}

void render_input_box(Game *game, Input_box *input_box) {
// Render appropriate surface based on state
if (input_box->is_active) {
SDL_BlitSurface(input_box->active, NULL, game->screen, &input_box->rect);
} else if (input_box->is_hovered) {
SDL_BlitSurface(input_box->hovered, NULL, game->screen, &input_box->rect);
} else {
SDL_BlitSurface(input_box->not_hovered, NULL, game->screen, &input_box->rect);
}
// Render centered text
render_txt(game, &input_box->txt);
}