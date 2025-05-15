//
// Created by lain on 5/13/25.
//

#ifndef SHITY_CODE_H
#define SHITY_CODE_H
#include "game/game.h"


typedef struct {
    SDL_Surface *piece;     // Surface for the puzzle piece
    SDL_Rect src_rect;      // Source rectangle in the scaled image
    SDL_Rect dest_rect;     // Current position on screen
    int correct_x;          // Correct x position
    int correct_y;          // Correct y position
    int is_dragging;        // Whether the piece is being dragged
} PuzzlePiece;

typedef struct {
    PuzzlePiece pieces[9];  // Array of 9 puzzle pieces
    SDL_Surface *background; // Pink background surface
    SDL_Surface *image;     // Scaled square jojo.png image
    SDL_Surface *reference_image; // Scaled-down reference image
    int is_solved;          // Whether the puzzle is solved
    Button *return_button;  // Button to return to main menu
    SDL_Rect table_rect;    // Rectangle for the holding table area (right)
    SDL_Rect scramble_table_rect; // Rectangle for the scramble table area (bottom)
} Puzzle;;


int shity_function(Game *game);

#endif //SHITY_CODE_H
