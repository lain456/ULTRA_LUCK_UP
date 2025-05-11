#include "button.h"
#include "../tools/tools.h"
#include "../text/txt.h"

int is_hovered(Game *game, SDL_Rect *rect) {
    return (game->x_mouse >= rect->x && game->x_mouse < rect->x + rect->w &&
            game->y_mouse >= rect->y && game->y_mouse < rect->y + rect->h);
}

int is_pressed(Game *game, SDL_Rect *rect) {
    return is_hovered(game, rect) && game->mouse_pressed;
}

int is_clicked(Game *game, SDL_Rect *rect) {
    return is_hovered(game, rect) && game->released_mouse;
}

int button_is_clicked(Game *game, Button *button) {
    return is_clicked(game, &button->b_rect);
}

void update_buttons(Game *game, Button *buttons, int size) {
    // Clear previous states
    for (int i = 0; i < size; i++) {
        buttons[i].isSelected = 0;
        buttons[i].isHovered = 0;
        buttons[i].isPressed = 0;
        buttons[i].isClicked = 0;
    }

    // Check for mouse hovering first
    int mouse_hovered = 0;
    for (int i = 0; i < size; i++) {
        if (is_hovered(game, &buttons[i].b_rect)) {
            buttons[i].isHovered = 1;
            buttons[i].isSelected = 1;
            game->selected_button_index = i;
            game->controller_active = 0;
            game->select = 0;
            mouse_hovered = 1;
            if (game->mouse_pressed) {
                buttons[i].isPressed = 1;
            }
            if (is_clicked(game, &buttons[i].b_rect)) {
                buttons[i].isClicked = 1;
            }
            break;
        }
    }

    // If no buttons are hovered, enable controller if no recent mouse motion
    if (!mouse_hovered) {
        // Only reset selected_button_index if controller is not active
        if (!game->controller_active) {
            game->selected_button_index = -1;
        }
        Uint32 current_time = SDL_GetTicks();
        if (current_time - game->last_mouse_motion > 1000) { // Increased to 1000ms
            game->controller_active = 1;
        }
        // Apply controller selection
        if (game->controller_active && game->select &&
            game->selected_button_index >= 0 && game->selected_button_index < size) {
            buttons[game->selected_button_index].isSelected = 1;
            buttons[game->selected_button_index].isHovered = 1;
        }
    }

    // Play hover sound
    for (int i = 0; i < size; i++) {
        if (buttons[i].isHovered && !buttons[i].b_switch) {
            Mix_PlayChannel(-1, game->sfx, 0);
        }
        buttons[i].b_switch = buttons[i].isHovered;
    }
}

void render_button(Game *game, Button *b, int hover) {
    if (b->isSelected || b->isHovered) {
        SDL_BlitSurface(b->hovered, NULL, game->screen, &b->rect);
    } else {
        SDL_BlitSurface(b->not_hovered, NULL, game->screen, &b->rect);
    }
    SDL_BlitSurface(b->txt.surf, NULL, game->screen, &b->txt.rect);
}

Button *create_button(Game *game, int x, int y, int h, int w, char* text, SDL_Color color, int type) {
    Button *button = (Button *)malloc(sizeof(Button));
    button->txt.writen = text;
    button->txt.font = game->main_font;
    button->b_switch = 0;
    button->x = x;
    button->y = y;
    button->w = w;
    button->h = h;
    button->rect = (SDL_Rect) { x, y, w, h };
    button->type = type;
    button->isHovered = 0;
    button->isPressed = 0;
    button->isClicked = 0;
    button->isSelected = 0;
    button->txt.color = color;
    button->x_center = x + (w/2);
    button->y_center = y + (h/2);
    button->not_hovered = scaleSurface(game->b_purple, w, h);
    button->hovered = scaleSurface(game->b_yellow, w, h);

    button->b_rect.x = x + (w/18);
    button->b_rect.y = y + (h*1/4);
    button->b_rect.w = button->w - (w/8);
    button->b_rect.h = button->h - (h*23/48);

    button->basic = create_color_surface(button->b_rect.w, button->b_rect.h, 255, 0, 0);

    button->txt.surf = TTF_RenderText_Blended(game->main_font, button->txt.writen, button->txt.color);
    button->txt.rect = (SDL_Rect){0, 0, button->txt.surf->w, button->txt.surf->h};
    button->txt.rect = (SDL_Rect){
        button->b_rect.x + (button->b_rect.w - button->txt.surf->w)/2,
        button->b_rect.y + (button->b_rect.h - button->txt.surf->h)/2,
        button->txt.surf->w,
        button->txt.surf->h
    };

    return button;
}

void render_buttons(Game *game, Button button[], int b_cnt) {
    for (int i = 0; i < b_cnt; i++) {
        render_button(game, &button[i], 1);
    }
}

void b_pos_update(Button *button, int new_x, int new_y) {
    int x_change = new_x - button->b_rect.x;
    int y_change = new_y - button->b_rect.y;

    button->b_rect.x += x_change;
    button->b_rect.y += y_change;
    button->rect.x += x_change;
    button->rect.y += y_change;
    button->txt.rect.x += x_change;
    button->txt.rect.y += y_change;
}

void y_order_buttons(Button *button, int margin, int b_ct) {
    for (int i = 1; i < b_ct; i++) {
        button[i].b_rect.y = button[i-1].b_rect.y + button[i-1].b_rect.h + margin;
        button[i].txt.rect.y = button[i].b_rect.y + (button[i].b_rect.h - button[i].txt.surf->h)/2;
        button[i].rect.y = button[i].b_rect.y + (button[i].b_rect.h - button[i].rect.h)/2;
    }
}

void x_order_buttons(Button *button, int margin, int b_ct) {
    for (int i = 1; i < b_ct; i++) {
        button[i].b_rect.x = button[i-1].b_rect.x + button[i-1].b_rect.w + margin;
        button[i].txt.rect.x = button[i].b_rect.x + (button[i].b_rect.w - button[i].txt.surf->w)/2;
        button[i].rect.x = button[i].b_rect.x + (button[i].b_rect.w - button[i].rect.w)/2;
    }
}