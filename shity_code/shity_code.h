#ifndef SHITY_CODE_H
#define SHITY_CODE_H
#include "game/game.h"

typedef enum {
    DIFFICULTY_EASY = 2,   // 2x2 grid
    DIFFICULTY_MEDIUM = 4, // 4x4 grid
    DIFFICULTY_HARD = 16   // 16x16 grid
} PuzzleDifficulty;

typedef struct {
    SDL_Surface *piece;     // Surface for the puzzle piece
    SDL_Rect src_rect;      // Source rectangle in the scaled image
    SDL_Rect dest_rect;     // Current position on screen
    int correct_x;          // Correct x position
    int correct_y;          // Correct y position
    int is_dragging;        // Whether the piece is being dragged
} PuzzlePiece;

typedef struct {
    PuzzlePiece *pieces;    // Dynamic array of puzzle pieces
    int grid_size;          // Grid size (e.g., 2 for 2x2, 4 for 4x4)
    int piece_count;        // Total number of pieces (grid_size * grid_size)
    SDL_Surface *background; // Pink background surface
    SDL_Surface *image;     // Scaled square jojo.png image
    SDL_Surface *reference_image; // Scaled-down reference image
    int is_solved;          // Whether the puzzle is solved
    Button *return_button;  // Button to return to main menu
    SDL_Rect table_rect;    // Rectangle for the holding table area (right)
    SDL_Rect scramble_table_rect; // Rectangle for the scramble table area (bottom)
    PuzzleDifficulty difficulty; // Current difficulty level
} Puzzle;

int shity_function(Game *game, PuzzleDifficulty difficulty);
SDL_Rect get_hovered_grid_spot(Game *game, Puzzle *puzzle);

#endif //SHITY_CODE_H