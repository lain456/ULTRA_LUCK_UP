#ifndef GAME_H
#define GAME_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// game window dimensions :
#define WIDTH 1080
#define HEIGHT 720

// SDL colors
#define BLACK (SDL_Color){0,0,0}
#define WHITE (SDL_Color){255,255,255}
#define GOLD (SDL_Color){255,215,0}

// bird dimensions in px :
#define BIRD_OG_WIDTH 829
#define BIRD_OG_HEIGHT 773

//paths used :
//basic config
#define BACK_PNG_PATH "../assets/graphics/basic/back.png"
#define BUTTON_PNG_PATH "../assets/graphics/menu/white.png"
#define BUTTON_PNG2_PATH "../assets/graphics/menu/yellow.png"
//************************************************************************************//
//bird config
#define BIRD_R01_PATH "../assets/graphics/characters/bird/flying to the right/b2.png"
#define BIRD_R02_PATH "../assets/graphics/characters/bird/flying to the right/b2.png"
#define BIRD_R03_PATH "../assets/graphics/characters/bird/flying to the right/b3.png"
#define BIRD_R04_PATH "../assets/graphics/characters/bird/flying to the right/b4.png"
#define BIRD_R05_PATH "../assets/graphics/characters/bird/flying to the right/b5.png"
#define BIRD_R06_PATH "../assets/graphics/characters/bird/flying to the right/b6.png"
#define BIRD_R07_PATH "../assets/graphics/characters/bird/flying to the right/b7.png"
//************************************************************************************//
#define BIRD_L01_PATH "../assets/graphics/characters/bird/flying to the left/b1.png"
#define BIRD_L02_PATH "../assets/graphics/characters/bird/flying to the left/b2.png"
#define BIRD_L03_PATH "../assets/graphics/characters/bird/flying to the left/b3.png"
#define BIRD_L04_PATH "../assets/graphics/characters/bird/flying to the left/b4.png"
#define BIRD_L05_PATH "../assets/graphics/characters/bird/flying to the left/b5.png"
#define BIRD_L06_PATH "../assets/graphics/characters/bird/flying to the left/b6.png"
#define BIRD_L07_PATH "../assets/graphics/characters/bird/flying to the left/b7.png"
//************************************************************************************//
//purple bird config
#define P_BIRD_R01_PATH "../assets/graphics/characters/p_bird/right/p1.png"
#define P_BIRD_R02_PATH "../assets/graphics/characters/p_bird/right/p2.png"
#define P_BIRD_R03_PATH "../assets/graphics/characters/p_bird/right/p3.png"
#define P_BIRD_R04_PATH "../assets/graphics/characters/p_bird/right/p4.png"
#define P_BIRD_R05_PATH "../assets/graphics/characters/p_bird/right/p5.png"
#define P_BIRD_R06_PATH "../assets/graphics/characters/p_bird/right/p6.png"
#define P_BIRD_R07_PATH "../assets/graphics/characters/p_bird/right/p7.png"
//************************************************************************************//
#define P_BIRD_L01_PATH "../assets/graphics/characters/p_bird/left/p1.png"
#define P_BIRD_L02_PATH "../assets/graphics/characters/p_bird/left/p2.png"
#define P_BIRD_L03_PATH "../assets/graphics/characters/p_bird/left/p3.png"
#define P_BIRD_L04_PATH "../assets/graphics/characters/p_bird/left/p4.png"
#define P_BIRD_L05_PATH "../assets/graphics/characters/p_bird/left/p5.png"
#define P_BIRD_L06_PATH "../assets/graphics/characters/p_bird/left/p6.png"
#define P_BIRD_L07_PATH "../assets/graphics/characters/p_bird/left/p7.png"
//************************************************************************************//
//font config :
#define FREDOKA_PATH "../assets/font/fredoka-one.one-regular.ttf"
//************************************************************************************//

#define LOGO_PATH "../assets/graphics/menu/main_logo.png"
#define LOGO2_PATH "../assets/graphics/menu/txt_logo.png"

#define MUSIC_PATH "../assets/audio/music.mp3"
#define HOVER_SFX_PATH "../assets/audio/hover.wav"

// gameplay :
#define SQUARE_SIZE 100
#define MOVE_SPEED 10
#define FPS 120
#define CYCLE 32
#define MAX_ENEMIES 2

typedef struct {
    int B9, B10, B11, B12, B13;
    int JX, JY, JSW;
    char Green[4], Red[4];
} ArduinoStatus;

typedef struct {
    // function input
    char *writen;
    SDL_Color color;
    TTF_Font *font;
    // function output - AKA requre treatment
    SDL_Surface *surf;
    SDL_Rect rect;
}Text;

typedef struct {
    int h,w,x,y,x_center,y_center,type;
    //prototype
    SDL_Rect b_rect;
    SDL_Surface *basic;
    SDL_Surface *final;
    // custom
    SDL_Rect rect;
    SDL_Surface * not_hovered;
    SDL_Surface * hovered;
    //text
    Text txt;
    //state
    int b_switch;
    int isHovered;
    int isPressed;
    int isClicked;
    int isSelected;                 // True if the button is selected
}Button;

typedef struct {
    // to be developed
    SDL_Surface *bg;
    SDL_Color color;
    SDL_Surface *fill;
    Button *button;
    SDL_Rect rect; // for the bg
    SDL_Rect s_rect;  // for the fill
    int val; // % ratio
    // button
    SDL_Surface *b;
    SDL_Surface *bh;
    int b_size;
    SDL_Rect b_rect;
    int is_clicked;
    int is_hovered;
    SDL_Color b_color;
    int x_dif;
} Slider;

typedef struct {
    // menu elements :
    Button *buttonlist;
    int selected_index;             // Index of the currently selected button
    int b_ct;
    Text *txtlist;
    int txt_ct;
    SDL_Surface *background;
    int t_margine,b_margine;
    Slider *slider_list;
    int s_ct,s_margine;
}Menu;

// creating nodes for our tree
typedef struct M_node {
    struct M_node *back;
    Menu *menu;
    int id;
    struct M_node *child_list;
}M_node;

typedef struct {
    SDLKey up;
    SDLKey down;
    SDLKey left;
    SDLKey right;
    SDLKey jump;
    SDLKey dash;
    float x_analog;
    float y_analog;
}Input;

typedef struct {
    //movements
    int x, y;
    float x_speed, y_speed;
    float x_accel, y_accel;
    //stats
    int health;
    int level; // Add level attribute
    // direction
    int move;
    int jump;
    int dash;
    int attack;
    int moveLeft,moveRight,moveUp,moveDown;
    // animation spritesheet WIP
    SDL_Rect rect;
    SDL_Rect h_rect;
    SDL_Surface *surface;
    SDL_Surface * flying_to_the_right[8];
    SDL_Surface * flying_to_the_left[8];
    // second player sprite sheet
    SDL_Surface * p_flying_to_the_right[8];
    SDL_Surface * p_flying_to_the_left[8];
    int index;
    int cycle;
    // fight animation WIP
    int player_num;
    // input ...crap to make customizable
    Input input;
    int jump_count;       // Number of jumps performed
    int max_jumps;
    int jump_trigger;
} Player;

// Platform structure
typedef struct {
    SDL_Rect rect; // Position and size
    SDL_Rect h_rect;
} Platform;

typedef struct {
    int serial_fd;                  // File descriptor for serial port
    char serial_buffer[256];        // Buffer for serial data
    int serial_buffer_len;          // Length of data in buffer
    Uint32 last_nav_time;           // Time of last navigation
    ArduinoStatus arduino_status;   // Current Arduino state
    ArduinoStatus prev_arduino_status; // Previous Arduino state for edge detection
    SDL_Event event;
    char *title;
    int width;
    int height;
    SDL_Surface *screen;
    int back;
    int fullscreen;
    int state;
    int quite;
    Player *player;
    Player *player2; // Added for future 2-player mode
    TTF_Font *mini_font;
    TTF_Font *main_font;
    TTF_Font *mid_font;
    TTF_Font *big_main_font;
    // mouse and button stuff
    int x_mouse,y_mouse,x_button_size,y_button_size,margin;
    int released_mouse;
    int mouse_pressed;
    SDL_Surface *b_yellow;
    SDL_Surface *b_purple;
    //slider stuff
    int x_slider_size,y_slider_size;
    SDL_Color slider_fill_color;
    SDL_Color slider_bg_color;
    SDL_Surface *slider_bg, *slider_fill, *b_slider;
    //often used surfaces will be here :
    // background
    SDL_Surface *background;
    SDL_Surface *black_surface;
    SDL_Surface *logo;
    // the real deal ...
    Menu *current_menu;
    M_node *current_node;
    int escape;
    // audio stuff
    Mix_Chunk *sfx; //single sfx wav format
    Mix_Music *music; // to listen to your bad taste in music
    int music_volume;
    int sfx_volume;
    //animation
    int frame;
    int cycle;
    int fps;
    int current_frame;  // this number cycles each time it reaches the fps ( it goes from one to 60 if fps is 60)
    time_t current_time;
    // Time fields
    time_t last_time_update; // Last time the time string was updated
    char time_string[32];   // Buffer for formatted time (HH:MM:SS)
    // FPS fields
    Uint32 last_fps_update; // Last time FPS was calculated (SDL ticks)
    int frame_count;        // Number of frames since last FPS update
    float current_fps;
    int selected_button_index;      // Index of the currently selected button
    Uint32 last_joystick_nav;       // Time of last joystick navigation (for cooldown)
    int joystick_nav_cooldown;      // Cooldown in ms for joystick navigation
    // FPS control
    Uint32 last_frame_time;
    int select;                     // Flag to indicate joystick selection
    int controller_active;          // Flag to indicate if controller navigation is active
    Uint32 last_mouse_motion;       // Time of last mouse motion event
}Game;

int gameplay(Game *game);
int pizza();

#endif //GAME_H