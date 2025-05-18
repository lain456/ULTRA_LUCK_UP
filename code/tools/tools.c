#include "tools.h"
#include "../text/txt.h"

SDL_Surface *create_color_surface(int w, int h, Uint8 r, Uint8 g, Uint8 b) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    if (!surface) {
        printf("Error: SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    return surface;
}

SDL_Surface *scaleSurface(SDL_Surface *surface, int width, int height) {
    if (!surface) {
        printf("Error: NULL surface in scaleSurface\n");
        return NULL;
    }
    SDL_Surface *scaledSurface = SDL_CreateRGBSurface(surface->flags, width, height, surface->format->BitsPerPixel,
                                                      surface->format->Rmask, surface->format->Gmask,
                                                      surface->format->Bmask, surface->format->Amask);
    if (!scaledSurface) {
        printf("Error: SDL_CreateRGBSurface failed in scaleSurface: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_SoftStretch(surface, NULL, scaledSurface, NULL);
    return scaledSurface;
}

SDL_Surface *create_blank_surface(int w, int h) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    if (!surface) {
        printf("Error: SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    SDL_SetAlpha(surface, SDL_SRCALPHA, 0);
    return surface;
}

Text *safe_create_txt(const char *text, TTF_Font *font, SDL_Color color, int x_pos, int y_pos) {
    Text *txt = (Text *)malloc(sizeof(Text));
    if (!txt) {
        printf("Error: Failed to allocate Text struct in safe_create_txt\n");
        return NULL;
    }
    txt->writen = strdup(text ? text : "");
    if (!txt->writen) {
        printf("Error: Failed to allocate text string in safe_create_txt\n");
        free(txt);
        return NULL;
    }
    txt->color = color;
    txt->font = font;
    txt->surf = NULL;
    txt->rect = (SDL_Rect){x_pos, y_pos, 0, 0};

    if (!font) {
        printf("Warning: NULL font in safe_create_txt, using empty surface\n");
        txt->surf = create_color_surface(1, 1, color.r, color.g, color.b);
        return txt;
    }

    txt->surf = TTF_RenderText_Solid(font, text ? text : "", color);
    if (!txt->surf) {
        printf("Warning: TTF_RenderText_Solid failed: %s\n", TTF_GetError());
        txt->surf = create_color_surface(1, 1, color.r, color.g, color.b);
        if (!txt->surf) {
            printf("Error: Failed to create fallback surface in safe_create_txt\n");
            free(txt->writen);
            free(txt);
            return NULL;
        }
    } else {
        txt->rect.w = txt->surf->w;
        txt->rect.h = txt->surf->h;
    }

    return txt;
}

void toggle_fullscreen(Game *game) {
    if (!game || !game->screen) {
        printf("Error: Invalid game or screen in toggle_fullscreen\n");
        return;
    }

    Uint32 flags = SDL_SWSURFACE;
    game->fullscreen = !game->fullscreen;
    if (game->fullscreen) {
        flags |= SDL_FULLSCREEN;
    }

    SDL_Surface *new_screen = SDL_SetVideoMode(game->width, game->height, 32, flags);
    if (!new_screen) {
        printf("Error: Failed to set video mode: %s\n", SDL_GetError());
        game->fullscreen = !game->fullscreen;
        return;
    }

    game->screen = new_screen;
    SDL_WM_GrabInput(game->fullscreen ? SDL_GRAB_OFF : SDL_GRAB_OFF);

    SDL_Flip(game->screen);
}

void draw_rect(SDL_Surface *surface, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b) {
    if (!surface) {
        printf("Error: NULL surface in draw_rect\n");
        return;
    }

    Uint32 color = SDL_MapRGB(surface->format, r, g, b);
    SDL_FillRect(surface, &(SDL_Rect){x, y, w, 1}, color);
    SDL_FillRect(surface, &(SDL_Rect){x, y + h - 1, w, 1}, color);
    SDL_FillRect(surface, &(SDL_Rect){x, y, 1, h}, color);
    SDL_FillRect(surface, &(SDL_Rect){x + w - 1, y, 1, h}, color);
}