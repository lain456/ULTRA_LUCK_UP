#include "shity_code.h"
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "button/button.h"
#include "tools/tools.h"

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

    int size = src->w < src->h ? src->w : src->h;
    size -= size % 3;

    SDL_Surface *cropped = SDL_CreateRGBSurface(
        0, size, size, src->format->BitsPerPixel,
        src->format->Rmask, src->format->Gmask, src->format->Bmask, src->format->Amask
    );
    if (!cropped) {
        printf("Error: Failed to create cropped surface: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Rect src_rect = {
        (src->w - size) / 2,
        (src->h - size) / 2,
        size, size
    };
    SDL_BlitSurface(src, &src_rect, cropped, NULL);
    return cropped;
}

static void init_puzzle(Game *game, Puzzle *puzzle, PuzzleDifficulty difficulty) {
    puzzle->difficulty = difficulty;
    puzzle->grid_size = difficulty; // Grid size matches difficulty (2, 4, or 16)
    puzzle->piece_count = puzzle->grid_size * puzzle->grid_size;

    // Allocate pieces array
    puzzle->pieces = malloc(sizeof(PuzzlePiece) * puzzle->piece_count);
    if (!puzzle->pieces) {
        printf("Error: Failed to allocate puzzle pieces\n");
        game->state = 1;
        return;
    }

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

    int PIECE_SIZE = puzzle->image->w / puzzle->grid_size;
    int PUZZLE_X = (WIDTH - PIECE_SIZE * puzzle->grid_size) / 2;
    int PUZZLE_Y = 120;

    // Initialize pieces
    for (int i = 0; i < puzzle->piece_count; i++) {
        int row = i / puzzle->grid_size;
        int col = i % puzzle->grid_size;

        puzzle->pieces[i].src_rect = (SDL_Rect){
            col * PIECE_SIZE, row * PIECE_SIZE, PIECE_SIZE, PIECE_SIZE
        };

        puzzle->pieces[i].correct_x = PUZZLE_X + col * PIECE_SIZE;
        puzzle->pieces[i].correct_y = PUZZLE_Y + row * PIECE_SIZE;

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

    puzzle->table_rect = (SDL_Rect){TABLE_X, TABLE_Y, TABLE_WIDTH, TABLE_HEIGHT};
    puzzle->scramble_table_rect = (SDL_Rect){SCRAMBLE_TABLE_X, SCRAMBLE_TABLE_Y, SCRAMBLE_TABLE_WIDTH, SCRAMBLE_TABLE_HEIGHT};

    puzzle->reference_image = scaleSurface(puzzle->image, REFERENCE_SIZE, REFERENCE_SIZE);
    if (!puzzle->reference_image) {
        printf("Error: Failed to create reference image: %s\n", SDL_GetError());
        game->state = 1;
        return;
    }

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
    for (int i = 0; i < puzzle->piece_count; i++) {
        if (puzzle->pieces[i].piece) {
            SDL_FreeSurface(puzzle->pieces[i].piece);
            puzzle->pieces[i].piece = NULL;
        }
    }
    if (puzzle->pieces) {
        free(puzzle->pieces);
        puzzle->pieces = NULL;
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

SDL_Rect get_hovered_grid_spot(Game *game, Puzzle *puzzle) {
    int PIECE_SIZE = puzzle->image->w / puzzle->grid_size;
    int PUZZLE_X = (WIDTH - PIECE_SIZE * puzzle->grid_size) / 2;
    int PUZZLE_Y = 120;

    int grid_x = (game->x_mouse - PUZZLE_X) / PIECE_SIZE;
    int grid_y = (game->y_mouse - PUZZLE_Y) / PIECE_SIZE;

    // Check if within grid bounds
    if (grid_x >= 0 && grid_x < puzzle->grid_size && grid_y >= 0 && grid_y < puzzle->grid_size) {
        return (SDL_Rect){
            PUZZLE_X + grid_x * PIECE_SIZE,
            PUZZLE_Y + grid_y * PIECE_SIZE,
            PIECE_SIZE,
            PIECE_SIZE
        };
    }

    // Return an invalid rect if not hovering over grid
    return (SDL_Rect){-1, -1, 0, 0};
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
                    if (puzzle->return_button && is_hovered(game, &puzzle->return_button->b_rect)) {
                        puzzle->return_button->isClicked = 1;
                    } else {
                        for (int i = puzzle->piece_count - 1; i >= 0; i--) {
                            if (is_hovered(game, &puzzle->pieces[i].dest_rect)) {
                                puzzle->pieces[i].is_dragging = 1;
                                break;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    game->released_mouse = 1;
                    game->mouse_pressed = 0;
                    for (int i = 0; i < puzzle->piece_count; i++) {
                        if (puzzle->pieces[i].is_dragging) {
                            puzzle->pieces[i].is_dragging = 0;
                            int PIECE_SIZE = puzzle->image->w / puzzle->grid_size;
                            int PUZZLE_X = (WIDTH - PIECE_SIZE * puzzle->grid_size) / 2;
                            int PUZZLE_Y = 120;
                            if (is_in_scramble_table(game, puzzle)) {
                                puzzle->pieces[i].dest_rect.x = game->x_mouse - PIECE_SIZE / 2;
                                puzzle->pieces[i].dest_rect.y = game->y_mouse - PIECE_SIZE / 2;
                                if (puzzle->pieces[i].dest_rect.x < puzzle->scramble_table_rect.x)
                                    puzzle->pieces[i].dest_rect.x = puzzle->scramble_table_rect.x;
                                if (puzzle->pieces[i].dest_rect.y < puzzle->scramble_table_rect.y)
                                    puzzle->pieces[i].dest_rect.y = puzzle->scramble_table_rect.y;
                                if (puzzle->pieces[i].dest_rect.x + PIECE_SIZE > puzzle->scramble_table_rect.x + puzzle->scramble_table_rect.w)
                                    puzzle->pieces[i].dest_rect.x = puzzle->scramble_table_rect.x + puzzle->scramble_table_rect.w - PIECE_SIZE;
                                if (puzzle->pieces[i].dest_rect.y + PIECE_SIZE > puzzle->scramble_table_rect.y + puzzle->scramble_table_rect.h)
                                    puzzle->pieces[i].dest_rect.y = puzzle->scramble_table_rect.y + puzzle->scramble_table_rect.h - PIECE_SIZE;
                            } else if (is_in_table(game, puzzle)) {
                                puzzle->pieces[i].dest_rect.x = game->x_mouse - PIECE_SIZE / 2;
                                puzzle->pieces[i].dest_rect.y = game->y_mouse - PIECE_SIZE / 2;
                                if (puzzle->pieces[i].dest_rect.x < puzzle->table_rect.x)
                                    puzzle->pieces[i].dest_rect.x = puzzle->table_rect.x;
                                if (puzzle->pieces[i].dest_rect.y < puzzle->table_rect.y)
                                    puzzle->pieces[i].dest_rect.y = puzzle->table_rect.y;
                                if (puzzle->pieces[i].dest_rect.x + PIECE_SIZE > puzzle->table_rect.x + puzzle->table_rect.w)
                                    puzzle->pieces[i].dest_rect.x = puzzle->table_rect.x + puzzle->table_rect.w - PIECE_SIZE;
                                if (puzzle->pieces[i].dest_rect.y + PIECE_SIZE > puzzle->table_rect.y + puzzle->table_rect.h)
                                puzzle->pieces[i].dest_rect.y = puzzle->table_rect.y + puzzle->table_rect.h - PIECE_SIZE;
                            } else {
                                int grid_x = (game->x_mouse - PUZZLE_X + PIECE_SIZE / 2) / PIECE_SIZE;
                                int grid_y = (game->y_mouse - PUZZLE_Y + PIECE_SIZE / 2) / PIECE_SIZE;
                                grid_x = grid_x < 0 ? 0 : (grid_x >= puzzle->grid_size ? puzzle->grid_size - 1 : grid_x);
                                grid_y = grid_y < 0 ? 0 : (grid_y >= puzzle->grid_size ? puzzle->grid_size - 1 : grid_y);
                                int new_x = PUZZLE_X + grid_x * PIECE_SIZE;
                                int new_y = PUZZLE_Y + grid_y * PIECE_SIZE;

                                int occupant = -1;
                                for (int j = 0; j < puzzle->piece_count; j++) {
                                    if (j != i && puzzle->pieces[j].dest_rect.x == new_x &&
                                        puzzle->pieces[j].dest_rect.y == new_y) {
                                        occupant = j;
                                        break;
                                    }
                                }

                                if (occupant == -1) {
                                    puzzle->pieces[i].dest_rect.x = new_x;
                                    puzzle->pieces[i].dest_rect.y = new_y;
                                } else {
                                    puzzle->pieces[i].dest_rect.x = game->x_mouse - PIECE_SIZE / 2;
                                    puzzle->pieces[i].dest_rect.y = game->y_mouse - PIECE_SIZE / 2;
                                }
                            }
                        }
                    }
                    if (puzzle->return_button && puzzle->return_button->isClicked && is_hovered(game, &puzzle->return_button->b_rect)) {
                        game->state = 1;
                    }
                    puzzle->return_button->isClicked = 0;
                }
                break;
            case SDL_MOUSEMOTION:
                for (int i = 0; i < puzzle->piece_count; i++) {
                    if (puzzle->pieces[i].is_dragging) {
                        int PIECE_SIZE = puzzle->image->w / puzzle->grid_size;
                        puzzle->pieces[i].dest_rect.x = game->x_mouse - PIECE_SIZE / 2;
                        puzzle->pieces[i].dest_rect.y = game->y_mouse - PIECE_SIZE / 2;
                    }
                }
                update_buttons(&game, puzzle->return_button , 1);
                break;
        }
    }
}

static void check_puzzle_solved(Puzzle *puzzle) {
    puzzle->is_solved = 1;
    for (int i = 0; i < puzzle->piece_count; i++) {
        if (puzzle->pieces[i].dest_rect.x != puzzle->pieces[i].correct_x ||
            puzzle->pieces[i].dest_rect.y != puzzle->pieces[i].correct_y) {
            puzzle->is_solved = 0;
            break;
        }
    }
}

static void render_puzzle(Game *game, Puzzle *puzzle) {
    SDL_BlitSurface(puzzle->background, NULL, game->screen, NULL);

    SDL_Rect reference_pos = {TABLE_X + (TABLE_WIDTH - REFERENCE_SIZE) / 2, TABLE_Y - REFERENCE_SIZE - 10, REFERENCE_SIZE, REFERENCE_SIZE};
    SDL_BlitSurface(puzzle->reference_image, NULL, game->screen, &reference_pos);

    SDL_FillRect(game->screen, &puzzle->table_rect, SDL_MapRGB(game->screen->format, 200, 200, 200));
    SDL_FillRect(game->screen, &puzzle->scramble_table_rect, SDL_MapRGB(game->screen->format, 200, 200, 200));

    // Highlight hovered grid spot
    SDL_Rect hovered = get_hovered_grid_spot(game, puzzle);
    if (hovered.x != -1) {
        draw_rect(game->screen, hovered.x, hovered.y, hovered.w, hovered.h, 0, 255, 0);
    }

    for (int i = 0; i < puzzle->piece_count; i++) {
        SDL_BlitSurface(puzzle->pieces[i].piece, NULL, game->screen, &puzzle->pieces[i].dest_rect);
    }

    render_buttons(game, puzzle->return_button , 1);
}

int shity_function(Game *game, PuzzleDifficulty difficulty) {
    Puzzle puzzle = {0};
    init_puzzle(game, &puzzle, difficulty);

    if (game->state == 1) {
        free_puzzle(&puzzle);
        return 1;
    }

    while (!game->quite && game->state == 2) {
        Uint32 frame_start = SDL_GetTicks();

        SDL_GetMouseState(&game->x_mouse, &game->y_mouse);
        game->released_mouse = 0;

        handle_puzzle_input(game, &puzzle);
        check_puzzle_solved(&puzzle);

        if (puzzle.is_solved) {
            game->state = 1;
        }

        render_puzzle(game, &puzzle);
        SDL_Flip(game->screen);

        Uint32 frame_time = SDL_GetTicks() - frame_start;
        Uint32 frame_duration = 1000 / game->fps;
        if (frame_time < frame_duration) {
            SDL_Delay(frame_duration - frame_time);
        }
    }

    free_puzzle(&puzzle);
    return 0;
}