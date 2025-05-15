//
// Created by lain on 4/3/25.
//

#include "tools.h"

  // For game->width, game->height
/*
SDL_Surface *create_black_surface() {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 800, 600, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    return surface;
}

SDL_Surface *create_red_surface() {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 800, 600, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
    return surface;
}

SDL_Surface *create_blue_surface() {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 800, 600, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 255));
    return surface;
}
*/


// random shit we might need
/*
    if (text) {
        SDL_SetAlpha(text, SDL_SRCALPHA, 255);
        printf("Text rendered: %dx%d\n", text->w, text->h);
    } else {
        printf("Text failed: %s\n", TTF_GetError());
    }

*/


SDL_Surface *create_color_surface(int w , int h , Uint8 r, Uint8 g, Uint8 b ) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w,h, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, r, g, b));
    return surface;
}



// scale surf :


SDL_Surface* scaleSurface(SDL_Surface* surface, int width, int height) {
    SDL_Surface* scaledSurface = SDL_CreateRGBSurface(surface->flags, width, height, surface->format->BitsPerPixel,
                                                      surface->format->Rmask, surface->format->Gmask,
                                                      surface->format->Bmask, surface->format->Amask);
    SDL_SoftStretch(surface, NULL, scaledSurface, NULL);
    return scaledSurface;
}



SDL_Surface *create_blank_surface(int w, int h) {
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
    if (!surface) {
        printf("SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
        return NULL;
    }

    // Fill with black (arbitrary, since alpha will make it invisible)/
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

    // Set fully transparent
    SDL_SetAlpha(surface, SDL_SRCALPHA, 0);

    return surface;
}





void toggle_fullscreen(Game *game) {
    Uint32 flags = SDL_SWSURFACE; // Base flags (software surface)

    // Toggle the fullscreen state
    game->fullscreen = !game->fullscreen;

    // Add SDL_FULLSCREEN flag if enabling fullscreen
    if (game->fullscreen) {
        flags |= SDL_FULLSCREEN;
    }

    // Recreate the display surface with new flags
    SDL_Surface *new_screen = SDL_SetVideoMode(game->width, game->height, 32, flags);

    // Check if the mode change was successful
    if (new_screen == NULL) {
        printf("Failed to set video mode: %s\n", SDL_GetError());
        game->fullscreen = !game->fullscreen; // Revert flag on failure
        return;
    }

    // Update the game's screen surface
    game->screen = new_screen;


    if (!game->fullscreen) {
        SDL_WM_GrabInput(SDL_GRAB_OFF);
    } else {
        // Optionally disable grab in fullscreen for menu navigation
        SDL_WM_GrabInput(SDL_GRAB_OFF); // Change to SDL_GRAB_ON if needed for gameplay
    }






    SDL_Flip(game->screen);
}



void draw_rect(SDL_Surface *surface, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b) {
    if (!surface) {
        printf("Error: NULL surface in draw_rect\n");
        return;
    }

    Uint32 color = SDL_MapRGB(surface->format, r, g, b);

    // Draw top line
    SDL_FillRect(surface, &(SDL_Rect){x, y, w, 1}, color);
    // Draw bottom line
    SDL_FillRect(surface, &(SDL_Rect){x, y + h - 1, w, 1}, color);
    // Draw left line
    SDL_FillRect(surface, &(SDL_Rect){x, y, 1, h}, color);
    // Draw right line
    SDL_FillRect(surface, &(SDL_Rect){x + w - 1, y, 1, h}, color);
}