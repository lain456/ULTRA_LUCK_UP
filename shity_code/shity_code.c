#include "shity_code.h"
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "button/button.h"
#include "tools/tools.h"

#define PUZZLE_GRID_SIZE 3
#define TABLE_X 850
#define TABLE_Y 100
#define TABLE_WIDTH 200
#define TABLE_HEIGHT 460
#define SCRAMBLE_TABLE_X 250
#define SCRAMBLE_TABLE_Y 520
#define SCRAMBLE_TABLE_WIDTH 300
#define SCRAMBLE_TABLE_HEIGHT 120
#define REFERENCE_SIZE 96
#define PUZZLE_TARGET_SIZE 300

static SDL_Surface* crop_to_square(SDL_Surface *src) {
    if (!src) return NULL;

    // Find the smaller dimension
    int size = src->w < src->h ? src->w : src->h;
    // Adjust to be divisible by 3
    size -= size % 3;

    // Create new square surface
    SDL_Surface *cropped = SDL_CreateRGBSurface(
        0, size, size, src->format->BitsPerPixel,
        src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask
    );
    if (!cropped) {
        printf("Error: Failed to create cropped surface: %s\n", SDL_GetError());
        return NULL;
    }

    // Center the crop
    SDL_Rect src_rect = {
        (src->w - size) / 2,
        (src->h - size) / 2,
        size, size
    };
    SDL_BlitSurface(src, &src_rect, cropped, NULL);
    return cropped;
}

static void init_puzzle(Game *game, Puzzle *puzzle) {
    // Create pink background
    puzzle->background = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
    if (!puzzle->background) {
        printf("Error: Failed to create background surface: %s\n", SDL_GetError());
        game->state = 1;
        return;
    }
    SDL_FillRect(puzzle->background, NULL, SDL_MapRGB(puzzle->background->format, 255, 192, 203));

    // Load and crop jojo.png
    SDL_Surface *temp_image = IMG_Load(JOJO_PATH);
    if (!temp_image) {
        printf("Error: Failed to load %s: %s\n", JOJO_PATH, IMG_GetError());
        game->state = 1;
        return;
    }

    SDL_Surface *cropped_image = crop_to_square(temp_image);
    SDL_FreeSurface(temp_image);
    if (!cropped_image) {
        game->state = 1;
        return;
    }

    // Scale to target puzzle size
    puzzle->image = scaleSurface(cropped_image, PUZZLE_TARGET_SIZE, PUZZLE_TARGET_SIZE);
    SDL_FreeSurface(cropped_image);
    if (!puzzle->image) {
        printf("Error: Failed to scale puzzle image: %s\n", SDL_GetError());
        game->state = 1;
        return;
    }

    int PIECE_SIZE = puzzle->image->w / 3;
    int PUZZLE_X = (WIDTH - PIECE_SIZE * PUZZLE_GRID_SIZE) / 2;
    int PUZZLE_Y = 120;

    // Initialize pieces
    for (int i = 0; i < 9; i++) {
        int row = i / PUZZLE_GRID_SIZE;
        int col = i % PUZZLE_GRID_SIZE;

        // Source rectangle in the scaled image
        puzzle->pieces[i].src_rect = (SDL_Rect){
            col * PIECE_SIZE, row * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE
        };

        // Correct position
        puzzle->pieces[i].correct_x = PUZZLE_X + col * PIECE_SIZE;
        puzzle->pieces[i].correct_y = PUZZLE_Y + row * PIECE_SIZE;

        // Initial position (random in scramble table)
        puzzle->pieces[i].dest_rect = (SDL_Rect){
            SCRAMBLE_TABLE_X + (rand() % (SCRAMBLE_TABLE_WIDTH - PIECE_SIZE)),
            SCRAMBLE_TABLE_Y + (rand() % (SCRAMBLE_TABLE_HEIGHT - PIECE_SIZE)),
            PIECE_SIZE, PIECE_SIZE
        };

        puzzle->pieces[i].piece = SDL_CreateRGBSurface(
            0, PIECE_SIZE, PIECE_SIZE, puzzle->image->format->BitsPerPixel,
            puzzle->image->format->Rmask, puzzle->image->format->Gmask,
            puzzle->image->format->Bmask, puzzle->image->format->Amask
        );
        if (!puzzle->pieces[i].piece) {
            printf("Error: Failed to create puzzle piece surface: %s\n", SDL_GetError());
            game->state = 1;
            return;
        }
        SDL_BlitSurface(puzzle->image, &puzzle->pieces[i].src_rect,
                       puzzle->pieces[i].piece, NULL);
        puzzle->pieces[i].is_dragging = 0;
    }

    puzzle->is_solved = 0;

    // Initialize tables
    puzzle->table_rect = (SDL_Rect){TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT};
    puzzle->scramble_table_rect = (SDL_Rect){SCRAMBLE_TABLE_X, SCRAMBLE_TABLE_Y, SCRAMBLE_TABLE_WIDTH, SCRAMBLE_TABLE_HEIGHT};

    // Initialize reference image
    puzzle->reference_image = scaleSurface(puzzle->image, REFERENCE_SIZE, REFERENCE_SIZE);
    if (!puzzle->reference_image) {
        printf("Error: Failed to create reference image: %s\n", SDL_GetError());
        game->state = 1;
        return;
    }

    // Create return button (positioned below holding table)
    puzzle->return_button = create_button(game,
        TABLE_X + (TABLE_WIDTH - game->x_button_size) / 2,
        TABLE_Y + TABLE_HEIGHT + 20,
        game->y_button_size, game->x_button_size, "Return", WHITE, 1);
    if (!puzzle->return_button) {
        printf("Error: Failed to create return button\n");
        game->state = 1;
    }
}

static void free_puzzle(Puzzle *puzzle) {
    for (int i = 0; i < 9; i++) {
        if (puzzle->pieces[i].piece) {
            SDL_FreeSurface(puzzle->pieces[i].piece);
            puzzle->pieces[i].piece = NULL;
        }
    }
    if (puzzle->background) {
        SDL_FreeSurface(puzzle->background);
        puzzle->background = NULL;
    }
    if (puzzle->image) {
        SDL_FreeSurface(puzzle->image);
        puzzle->image = NULL;
    }
    if (puzzle->reference_image) {
        SDL_FreeSurface(puzzle->reference_image);
        puzzle->reference_image = NULL;
    }
    if (puzzle->return_button) {
        SDL_FreeSurface(puzzle->return_button->not_hovered);
        SDL_FreeSurface(puzzle->return_button->hovered);
        SDL_FreeSurface(puzzle->return_button->txt.surf);
        free(puzzle->return_button);
        puzzle->return_button = NULL;
    }
}

static int is_in_table(Game *game, Puzzle *puzzle) {
    return (game->x_mouse >= puzzle->table_rect.x &&
            game->x_mouse < puzzle->table_rect.x + puzzle->table_rect.w &&
            game->y_mouse >= puzzle->table_rect.y &&
            game->y_mouse < puzzle->table_rect.y + puzzle->table_rect.h);
}

static int is_in_scramble_table(Game *game, Puzzle *puzzle) {
    return (game->x_mouse >= puzzle->scramble_table_rect.x &&
            game->x_mouse < puzzle->scramble_table_rect.x + puzzle->scramble_table_rect.w &&
            game->y_mouse >= puzzle->scramble_table_rect.y &&
            game->y_mouse < puzzle->scramble_table_rect.y + puzzle->scramble_table_rect.h);
}

static void handle_puzzle_input(Game *game, Puzzle *puzzle) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                game->quite = 1;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    game->state = 1;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    game->mouse_pressed = 1;
                    for (int i = 8; i >= 0; i--) { // Topmost piece
                        // HITBOX: Checks if mouse (x_mouse, y_mouse) is within piece's dest_rect (x, y, PIECE_SIZE, PIECE_SIZE)
                        if (is_hovered(game, &puzzle->pieces[i].dest_rect)) {
                            puzzle->pieces[i].is_dragging = 1;
                            break;
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    game->released_mouse = 1;
                    game->mouse_pressed = 0;
                    for (int i = 0; i < 9; i++) {
                        if (puzzle->pieces[i].is_dragging) {
                            puzzle->pieces[i].is_dragging = 0;
                            int PIECE_SIZE = puzzle->image->w / 3;
                            int PUZZLE_X = (WIDTH - PIECE_SIZE * PUZZLE_GRID_SIZE) / 2;
                            int PUZZLE_Y = 120;
                            // Check placement
                            if (is_in_scramble_table(game, puzzle)) {
                                // Place freely in scramble table
                                puzzle->pieces[i].dest_rect.x = game->x_mouse - PIECE_SIZE / 2;
                                puzzle->pieces[i].dest_rect.y = game->y_mouse - PIECE_SIZE / 2;
                                // Clamp to scramble table bounds
                                if (puzzle->pieces[i].dest_rect.x < puzzle->scramble_table_rect.x)
                                    puzzle->pieces[i].dest_rect.x = puzzle->scramble_table_rect.x;
                                if (puzzle->pieces[i].dest_rect.y < puzzle->scramble_table_rect.y)
                                    puzzle->pieces[i].dest_rect.y = puzzle->scramble_table_rect.y;
                                if (puzzle->pieces[i].dest_rect.x + PIECE_SIZE > puzzle->scramble_table_rect.x + puzzle->scramble_table_rect.w)
                                    puzzle->pieces[i].dest_rect.x = puzzle->scramble_table_rect.x + puzzle->scramble_table_rect.w - PIECE_SIZE;
                                if (puzzle->pieces[i].dest_rect.y + PIECE_SIZE > puzzle->scramble_table_rect.y + puzzle->scramble_table_rect.h)
                                puzzle->pieces[i].dest_rect.y = puzzle->scramble_table_rect.y + puzzle->scramble_table_rect.h - PIECE_SIZE;
                            } else if (is_in_table(game, puzzle)) {
                                // Place freely in holding table
                                puzzle->pieces[i].dest_rect.x = game->x_mouse - PIECE_SIZE / 2;
                                puzzle->pieces[i].dest_rect.y = game->y_mouse - PIECE_SIZE / 2;
                                // Clamp to holding table bounds
                                if (puzzle->pieces[i].dest_rect.x < puzzle->table_rect.x)
                                    puzzle->pieces[i].dest_rect.x = puzzle->table_rect.x;
                                if (puzzle->pieces[i].dest_rect.y < puzzle->table_rect.y)
                                    puzzle->pieces[i].dest_rect.y = puzzle->table_rect.y;
                                if (puzzle->pieces[i].dest_rect.x + PIECE_SIZE > puzzle->table_rect.x + puzzle->table_rect.w)
                                    puzzle->pieces[i].dest_rect.x = puzzle->table_rect.x + puzzle->table_rect.w - PIECE_SIZE;
                                if (puzzle->pieces[i].dest_rect.y + PIECE_SIZE > puzzle->table_rect.y + puzzle->table_rect.h)
                                    puzzle->pieces[i].dest_rect.y = puzzle->table_rect.y + puzzle->table_rect.h - PIECE_SIZE;
                            } else {
                                // Snap to nearest grid position
                                int grid_x = (game->x_mouse - PUZZLE_X + PIECE_SIZE / 2) / PIECE_SIZE;
                                int grid_y = (game->y_mouse - PUZZLE_Y + PIECE_SIZE / 2) / PIECE_SIZE;
                                grid_x = grid_x < 0 ? 0 : (grid_x > 2 ? 2 : grid_x);
                                grid_y = grid_y < 0 ? 0 : (grid_y > 2 ? 2 : grid_y);
                                int new_x = PUZZLE_X + grid_x * PIECE_SIZE;
                                int new_y = PUZZLE_Y + grid_y * PIECE_SIZE;

                                // Check for overlap in grid
                                int occupant = -1;
                                for (int j = 0; j < 9; j++) {
                                    if (j != i && puzzle->pieces[j].dest_rect.x == new_x &&
                                        puzzle->pieces[j].dest_rect.y == new_y) {
                                        occupant = j;
                                        break;
                                    }
                                }
                                if (occupant != -1) {
                                    // Swap positions
                                    SDL_Rect temp = puzzle->pieces[occupant].dest_rect;
                                    puzzle->pieces[occupant].dest_rect = puzzle->pieces[i].dest_rect;
                                    puzzle->pieces[i].dest_rect = (SDL_Rect){new_x, new_y, PIECE_SIZE, PIECE_SIZE};
                                } else {
                                    // Place in empty cell
                                    puzzle->pieces[i].dest_rect = (SDL_Rect){new_x, new_y, PIECE_SIZE, PIECE_SIZE};
                                }
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                game->x_mouse = e.motion.x;
                game->y_mouse = e.motion.y;
                game->last_mouse_motion = SDL_GetTicks();
                game->controller_active = 0;
                for (int i = 0; i < 9; i++) {
                    if (puzzle->pieces[i].is_dragging) {
                        int PIECE_SIZE = puzzle->image->w / 3;
                        // Center piece under cursor
                        puzzle->pieces[i].dest_rect.x = game->x_mouse - (PIECE_SIZE / 2);
                        puzzle->pieces[i].dest_rect.y = game->y_mouse - (PIECE_SIZE / 2);
                    }
                }
                break;
        }
    }
}

static void check_puzzle_solved(Puzzle *puzzle) {
    puzzle->is_solved = 1;
    for (int i = 0; i < 9; i++) {
        if (puzzle->pieces[i].dest_rect.x != puzzle->pieces[i].correct_x ||
            puzzle->pieces[i].dest_rect.y != puzzle->pieces[i].correct_y) {
            puzzle->is_solved = 0;
            break;
        }
    }
}

static void render_puzzle(Game *game, Puzzle *puzzle) {
    // Render background
    SDL_BlitSurface(puzzle->background, NULL, game->screen, NULL);

    int PIECE_SIZE = puzzle->image->w / 3;
    int PUZZLE_X = (WIDTH - PIECE_SIZE * PUZZLE_GRID_SIZE) / 2;
    int PUZZLE_Y = 120;

    // Render reference image (top left)
    SDL_Rect ref_rect = {10, 10, REFERENCE_SIZE, REFERENCE_SIZE};
    SDL_BlitSurface(puzzle->reference_image, NULL, game->screen, &ref_rect);

    // Render scramble table (bottom)
    SDL_FillRect(game->screen, &puzzle->scramble_table_rect, SDL_MapRGB(game->screen->format, 100, 100, 100));
    draw_rect(game->screen, puzzle->scramble_table_rect.x, puzzle->scramble_table_rect.y,
              puzzle->scramble_table_rect.w, puzzle->scramble_table_rect.h, 255, 255, 255);

    // Render holding table (right)
    SDL_FillRect(game->screen, &puzzle->table_rect, SDL_MapRGB(game->screen->format, 100, 100, 100));
    draw_rect(game->screen, puzzle->table_rect.x, puzzle->table_rect.y,
              puzzle->table_rect.w, puzzle->table_rect.h, 255, 255, 255);

    // Render grid
    for (int i = 0; i <= PUZZLE_GRID_SIZE; i++) {
        // Vertical lines
        draw_rect(game->screen, PUZZLE_X + i * PIECE_SIZE, PUZZLE_Y,
                  1, PIECE_SIZE * PUZZLE_GRID_SIZE, 255, 255, 255);
        // Horizontal lines
        draw_rect(game->screen, PUZZLE_X, PUZZLE_Y + i * PIECE_SIZE,
                  PIECE_SIZE * PUZZLE_GRID_SIZE, 1, 255, 255, 255);
    }

    // Render pieces
    for (int i = 0; i < 9; i++) {
        SDL_BlitSurface(puzzle->pieces[i].piece, NULL, game->screen,
                       &puzzle->pieces[i].dest_rect);
        // Draw border around pieces
        draw_rect(game->screen, puzzle->pieces[i].dest_rect.x, puzzle->pieces[i].dest_rect.y,
                 puzzle->pieces[i].dest_rect.w, puzzle->pieces[i].dest_rect.h,
                 255, 255, 255);
    }
    update_buttons(game, puzzle->return_button, 1);
    render_button(game, puzzle->return_button, 1);
}

int shity_function(Game *game) {
    printf("Puzzle function started\n");

    Puzzle puzzle = {0};
    init_puzzle(game, &puzzle);
    if (game->state == 1) { // Failed to load image or other initialization error
        free_puzzle(&puzzle);
        return 0;
    }

    srand(SDL_GetTicks()); // Seed random for shuffling

    while (game->state == 2 && !game->quite) {
        Uint32 frame_start = SDL_GetTicks();

        SDL_GetMouseState(&game->x_mouse, &game->y_mouse);
        game->released_mouse = 0;
        game->mouse_pressed = 0;

        handle_puzzle_input(game, &puzzle);

        if (puzzle.return_button->isClicked) {
            game->state = 1;
            break;
        }

        check_puzzle_solved(&puzzle);
        if (puzzle.is_solved) {
            printf("Puzzle solved!\n");
            game->state = 1;
            break;
        }

        render_puzzle(game, &puzzle);

        SDL_Flip(game->screen);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < 1000 / game->fps) {
            SDL_Delay(1000 / game->fps - frame_time);
        }
    }

    free_puzzle(&puzzle);
    printf("Puzzle function ended\n");
    return 69;
}