//
// Created by lain on 4/23/25.
//
//#include "shity_code.h"














#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

void load_backgrounds(Background backgrounds[]) {
    const char *bg_files[] = {"image/m2.png", "image/m1.png", "image/m3.png"};
    for (int i = 0; i < 3; i++) {
        backgrounds[i].background = IMG_Load(bg_files[i]);
        if (!backgrounds[i].background) {
            printf("Error loading background %i: %s\n", i, IMG_GetError());
            exit(1);
        }
        // Initialize camera_pos here
        //backgrounds[i].camera_pos.x = 0;
        //backgrounds[i].camera_pos.y = 0;
        //backgrounds[i].camera_pos.w = 1920;
        //backgrounds[i].camera_pos.h = 1080;
        //backgrounds[i].direction = 0;
    }
}

void load_buttons(Button buttons[]) {
    const char *inactive_files[] = {
        "image/p2.png", "image/s2.png", "image/h2.png",
        "image/i2.png", "image/e2.png", "image/r1.png", "image/next1.png",
    };

    const char *active_files[] = {
        "image/p1.png", "image/s1.png", "image/h1.png",
        "image/i1.png", "image/e1.png", "image/r2.png", "image/next2.png",
    };

    int screen_width = 1920;
    int button_width = 200;
    int center_x = (((screen_width - button_width) / 2) + 100);

    int button_y_positions[] = {200, 400, 500, 600, 700, 900, 700};

    for (int i = 0; i < 7; i++) {
        buttons[i].button_inactive = IMG_Load(inactive_files[i]);
        buttons[i].button_active = IMG_Load(active_files[i]);

        if (!buttons[i].button_inactive || !buttons[i].button_active) {
            printf("Error loading button %d: %s\n", i, IMG_GetError());
            exit(1);
        }

        Uint32 colorkey = SDL_MapRGB(buttons[i].button_inactive->format, 255, 0, 255);
        SDL_SetColorKey(buttons[i].button_inactive, SDL_SRCCOLORKEY, colorkey);
        SDL_SetColorKey(buttons[i].button_active, SDL_SRCCOLORKEY, colorkey);

        buttons[i].position.x = center_x - (buttons[i].button_inactive->w / 2);
        buttons[i].position.y = button_y_positions[i];
        buttons[i].is_hovered = 0;
    }
}

int button_clicked(Button btn, SDL_Event event) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (x >= btn.position.x && x <= btn.position.x + btn.button_inactive->w &&
            y >= btn.position.y && y <= btn.position.y + btn.button_inactive->h) {
            return 1;
        }
    }
    return 0;
}

void chargement_images(SDL_Surface **S, char *path) {
    *S = IMG_Load(path);
    if (!(*S)) {
        printf("Erreur chargement: %s\n", IMG_GetError());
    }
}

void hover_button(Button *btn, SDL_Event event) {
    if (event.type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        btn->is_hovered = (x >= btn->position.x && x <= btn->position.x + btn->button_inactive->w &&
                           y >= btn->position.y && y <= btn->position.y + btn->button_inactive->h);
    }
}

SDL_Surface* scaleSurface(SDL_Surface* surface, int width, int height) {
    if (!surface) return NULL;

    SDL_Surface* resized = SDL_CreateRGBSurface(
        SDL_SWSURFACE, width, height, surface->format->BitsPerPixel,
        surface->format->Rmask, surface->format->Gmask,
        surface->format->Bmask, surface->format->Amask
    );

    if (!resized) {
        printf("Erreur creation surface redimensionnée : %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Rect src_rect = {0, 0, surface->w, surface->h};
    SDL_Rect dst_rect = {0, 0, width, height};

    SDL_SoftStretch(surface, &src_rect, resized, &dst_rect);

    return resized;
}

void render_menu(SDL_Surface *screen, Background background, Button buttons[], int buttons_visible[], int current_background, TTF_Font *font, SDL_Color color) {
    SDL_Surface *logo_image = NULL;
    SDL_Surface *logo_name = NULL;
    SDL_Surface *game_name = NULL;
    SDL_Rect textRect = {800, 75, 0, 0};
    SDL_Rect logo = {10, 700, 421, 298};
    SDL_Rect nlogo = {60, 480, 298, 421};

    Uint32 startTime = SDL_GetTicks();
    char timeText[50];
    SDL_Color textColor = {0, 0, 0};
    SDL_BlitSurface(background.background, NULL, screen, NULL);

    if (current_background == 0 && font != NULL) {
        game_name = TTF_RenderText_Solid(font, "Hunter x Bird", color);
        if (game_name) {
            SDL_BlitSurface(game_name, NULL, screen, &textRect);
            SDL_FreeSurface(game_name);
        } else {
            printf("Erreur rendu texte : %s\n", TTF_GetError());
        }
    }

    if (current_background == 0){
        chargement_images(&logo_image, "/home/gastton/Documents/cdg/image1/logo.png");
        chargement_images(&logo_name, "/home/gastton/Documents/cdg/image1/NLOGO.png");

        if (logo_image) {
            SDL_Surface *scaled_logo = scaleSurface(logo_image, 421, 298);
            if (scaled_logo) {
                SDL_BlitSurface(scaled_logo, NULL, screen, &logo);
                SDL_FreeSurface(scaled_logo);
            }
            SDL_FreeSurface(logo_image);
        }

        if (logo_name) {
            SDL_Surface *scaled_nlogo = scaleSurface(logo_name, 298, 421);
            if (scaled_nlogo) {
                SDL_BlitSurface(scaled_nlogo, NULL, screen, &nlogo);
                SDL_FreeSurface(scaled_nlogo);
            }
            SDL_FreeSurface(logo_name);
        }
    }

    if (current_background == 1){
        render_time(screen, font, color, startTime);
    }

    for (int i = 0; i < 7; i++) {
        if (buttons_visible[i]) {
            SDL_Surface *button_img = buttons[i].is_hovered ? buttons[i].button_active : buttons[i].button_inactive;
            SDL_BlitSurface(button_img, NULL, screen, &buttons[i].position);
        }
    }

    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

char* get_user_input(SDL_Surface *screen, TTF_Font *font, SDL_Color color) {
    static char input_text[32] = "";
    SDL_Event event;
    int done = 0;

    SDL_Rect input_box = { 600, 400, 700, 60 };
    SDL_EnableUNICODE(1);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    done = 1;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int len = strlen(input_text);
                    if (len > 0) input_text[len - 1] = '\0';
                } else {
                    if (strlen(input_text) < sizeof(input_text) - 1) {
                        char ch = (char)event.key.keysym.unicode;
                        if (ch >= 32 && ch <= 126) {
                            int len = strlen(input_text);
                            input_text[len] = ch;
                            input_text[len + 1] = '\0';
                        }
                    }
                }
            }
        }

        SDL_Rect border = { input_box.x - 2, input_box.y - 2, input_box.w + 4, input_box.h + 4 };
        SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 255, 255, 255));

        SDL_FillRect(screen, &input_box, SDL_MapRGB(screen->format, 30, 30, 30));

        SDL_Surface *text_surface = TTF_RenderText_Solid(font, input_text, color);
        if (text_surface) {
            SDL_Rect text_pos = { input_box.x + 10, input_box.y + 15 };
            SDL_BlitSurface(text_surface, NULL, screen, &text_pos);
            SDL_FreeSurface(text_surface);
        }

        SDL_Flip(screen);
        SDL_Delay(16);
    }

    SDL_EnableUNICODE(0);
    return input_text;
}

void render_time(SDL_Surface *screen, TTF_Font *font, SDL_Color color, Uint32 startTime) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 seconds = currentTime / 1000;
    printf("%d   ", currentTime);
    Uint32 minutes = seconds / 60;
    seconds %= 60;

    char timeStr[20];
    snprintf(timeStr, sizeof(timeStr), "Time: %02d:%02d", minutes, seconds);
    printf(" %02d:%02d", minutes, seconds);
    SDL_Surface *timeSurface = TTF_RenderText_Solid(font, timeStr, color);

    if (timeSurface) {
        SDL_Rect timeRect = {20, 20, 0, 0};
        SDL_BlitSurface(timeSurface, NULL, screen, &timeRect);
    }
}

int initBackground(Background *b, char *path) {
    b->background = IMG_Load(path);

    if(!b->background) {
        printf("Unable to load background images: %s\n", IMG_GetError());
        return 0;
    }

    // Initialize camera position
    b->camera_pos.x = 0;
    b->camera_pos.y = 0;
    b->camera_pos.w = SCREEN_WIDTH;
    b->camera_pos.h = SCREEN_HEIGHT;

    b->direction = 0;
    return 1;
}

void scrolling(Background *b, int dx, int dy) {
    switch(b->direction) {
        case 0: // Right
            if(b->camera_pos.x + b->camera_pos.w < b->background->w)  // Changed from bgLayer1 to background
                b->camera_pos.x += dx;
            break;
        case 1: // Left
            if(b->camera_pos.x > 0)
                b->camera_pos.x -= dx;
            break;
        case 2: // Up
            if(b->camera_pos.y > 0)
                b->camera_pos.y -= dy;
            break;
        case 3: // Down
            if(b->camera_pos.y + b->camera_pos.h < b->background->h)  // Changed from bgLayer1 to background
                b->camera_pos.y += dy;
            break;
    }
}

void renderBackground(SDL_Surface *screen, Background *b) {
    // Create rects for rendering
    SDL_Rect dest = {0, 0, 0, 0};
    SDL_BlitSurface(b->background, &b->camera_pos, screen, &dest);
}