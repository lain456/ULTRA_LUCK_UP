#include "input.h"
#include "../tools/tools.h"
#include "../text/txt.h"
#include "../button/button.h"

Input_box *create_input_box(Game *game, int x, int y, int w, int h, char *initial_text, SDL_Color text_color, int max_length) {
    if (!game->b_purple || !game->b_yellow || !game->main_font) {
        printf("Error: Missing required resources (b_purple, b_yellow, or main_font) in create_input_box\n");
        return NULL;
    }

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
    input_box->cursor_visible = 0;
    input_box->last_cursor_toggle = SDL_GetTicks();

    input_box->not_hovered = scaleSurface(game->b_purple, w, h);
    input_box->hovered = scaleSurface(game->b_yellow, w, h);
    input_box->active = create_color_surface(w, h, 200, 200, 200);

    if (!input_box->not_hovered || !input_box->hovered || !input_box->active) {
        printf("Error: Failed to create surfaces for input_box\n");
        if (input_box->not_hovered) SDL_FreeSurface(input_box->not_hovered);
        if (input_box->hovered) SDL_FreeSurface(input_box->hovered);
        if (input_box->active) SDL_FreeSurface(input_box->active);
        free(input_box);
        return NULL;
    }

    Text *txt = safe_create_txt(initial_text[0] ? initial_text : " ", game->main_font, text_color, x, y);
    if (!txt || !txt->surf) {
        printf("Error: Failed to create text for input_box\n");
        SDL_FreeSurface(input_box->not_hovered);
        SDL_FreeSurface(input_box->hovered);
        SDL_FreeSurface(input_box->active);
        free(input_box);
        if (txt) free(txt);
        return NULL;
    }
    input_box->txt = *txt;
    free(txt);

    input_box->txt.rect.x = x + (w - input_box->txt.surf->w) / 2;
    input_box->txt.rect.y = y + (h - input_box->txt.surf->h) / 2;

    return input_box;
}

void update_input_box(Game *game, Input_box *input_box) {
    if (!input_box || !input_box->txt.surf) {
        printf("Warning: Invalid input_box or txt.surf in update_input_box\n");
        return;
    }

    input_box->is_hovered = is_hovered(game, &input_box->rect);

    if (is_clicked(game, &input_box->rect)) {
        input_box->is_active = 1;
    } else if (game->mouse_pressed && !input_box->is_hovered) {
        input_box->is_active = 0;
    }

    if (input_box->is_active && game->event.type == SDL_KEYDOWN) {
        SDLKey key = game->event.key.keysym.sym;
        if (key == SDLK_BACKSPACE && strlen(input_box->value) > 0) {
            input_box->value[strlen(input_box->value) - 1] = '\0';
        } else if (key == SDLK_RETURN) {
            printf("Input saved: %s\n", input_box->value);
            FILE *file = fopen("player_name.txt", "w");
            if (file) {
                fprintf(file, "%s\n", input_box->value);
                fclose(file);
            } else {
                printf("Warning: Failed to save input to file\n");
            }
        } else if (key >= SDLK_a && key <= SDLK_z && strlen(input_box->value) < input_box->max_length) {
            char c = (char)(key - SDLK_a + 'a');
            if (game->event.key.keysym.mod & KMOD_SHIFT) {
                c = c - 32;
            }
            char temp[2] = {c, '\0'};
            strncat(input_box->value, temp, input_box->max_length - strlen(input_box->value));
        }
        input_box->cursor_visible = 1;
        input_box->last_cursor_toggle = SDL_GetTicks();
    }

    if (input_box->is_active) {
        Uint32 current_time = SDL_GetTicks();
        if (current_time - input_box->last_cursor_toggle >= 500) {
            input_box->cursor_visible = !input_box->cursor_visible;
            input_box->last_cursor_toggle = current_time;
        }
    } else {
        input_box->cursor_visible = 0;
    }

    char display_text[258];
    snprintf(display_text, sizeof(display_text), "%s%s", input_box->value[0] ? input_box->value : " ", input_box->is_active && input_box->cursor_visible ? "|" : "");
    update_txt(&input_box->txt, display_text, input_box->txt.color, NULL);
    if (!input_box->txt.surf) {
        printf("Warning: Failed to update text in input_box\n");
        return;
    }
    input_box->txt.rect.x = input_box->rect.x + (input_box->rect.w - input_box->txt.surf->w) / 2;
    input_box->txt.rect.y = input_box->rect.y + (input_box->rect.h - input_box->txt.surf->h) / 2;
}

void update_input_boxes(Game *game, Input_box *input_boxes, int size) {
    for (int i = 0; i < size; i++) {
        update_input_box(game, &input_boxes[i]);
    }
}

void render_input_box(Game *game, Input_box *input_box) {
    if (!input_box || !game->screen) {
        printf("Warning: Invalid input_box or screen in render_input_box\n");
        return;
    }

    if (input_box->is_active && input_box->active) {
        SDL_BlitSurface(input_box->active, NULL, game->screen, &input_box->rect);
    } else if (input_box->is_hovered && input_box->hovered) {
        SDL_BlitSurface(input_box->hovered, NULL, game->screen, &input_box->rect);
    } else if (input_box->not_hovered) {
        SDL_BlitSurface(input_box->not_hovered, NULL, game->screen, &input_box->rect);
    }
    if (input_box->txt.surf) {
        render_txt(game, &input_box->txt);
    }
}

void render_input_boxes(Game *game, Input_box *input_boxes, int i_cnt) {
    for (int i = 0; i < i_cnt; i++) {
        render_input_box(game, &input_boxes[i]);
    }
}

void free_input_box(Input_box *input_box) {
    if (input_box) {
        if (input_box->not_hovered) SDL_FreeSurface(input_box->not_hovered);
        if (input_box->hovered) SDL_FreeSurface(input_box->hovered);
        if (input_box->active) SDL_FreeSurface(input_box->active);
        if (input_box->txt.surf) SDL_FreeSurface(input_box->txt.surf);
        free(input_box);
    }
}

int input_box_is_clicked(Game *game, Input_box *input_box) {
    return is_clicked(game, &input_box->rect);
}

void i_pos_update(Input_box *input_box, int new_x, int new_y) {
    if (!input_box) return;
    int x_change = new_x - input_box->rect.x;
    int y_change = new_y - input_box->rect.y;

    input_box->rect.x += x_change;
    input_box->rect.y += y_change;
    input_box->txt.rect.x += x_change;
    input_box->txt.rect.y += y_change;
}

void x_order_input_boxes(Input_box *input_boxes, int margin, int i_ct) {
    for (int i = 1; i < i_ct; i++) {
        input_boxes[i].rect.x = input_boxes[i-1].rect.x + input_boxes[i-1].rect.w + margin;
        input_boxes[i].txt.rect.x = input_boxes[i].rect.x + (input_boxes[i].rect.w - input_boxes[i].txt.surf->w) / 2;
    }
}

void y_order_input_boxes(Input_box *input_boxes, int margin, int i_ct) {
    for (int i = 1; i < i_ct; i++) {
        input_boxes[i].rect.y = input_boxes[i-1].rect.y + input_boxes[i-1].rect.h + margin;
        input_boxes[i].txt.rect.y = input_boxes[i].rect.y + (input_boxes[i].rect.h - input_boxes[i].txt.surf->h) / 2;
    }
}