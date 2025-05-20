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
#define YELLOW (SDL_Color){255,255,0}
#define LIGHT_YELLOW (SDL_Color){255,255,153}

// bird dimensions in px :
#define BIRD_OG_WIDTH 829
#define BIRD_OG_HEIGHT 773

// paths used :
#define GOOD_BG "../assets/graphics/menu/background.png"
#define SIDE_SCROLLING_BG "../assets/graphics/basic/side_ways scrolling_bg.png"
// basic config
#define BACK_PNG_PATH "../assets/graphics/basic/back.png"
#define BUTTON_PNG_PATH "../assets/graphics/menu/white.png"
#define BUTTON_PNG2_PATH "../assets/graphics/menu/yellow.png"
//************************************************************************************//
// bird config
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
// purple bird config
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
// font config :
#define FREDOKA_PATH "../assets/font/fredoka-one.one-regular.ttf"
//************************************************************************************//

#define LOGO_PATH "../assets/graphics/menu/main_logo.png"
#define LOGO2_PATH "../assets/graphics/menu/txt_logo.png"
#define JOJO_PATH "../assets/graphics/basic/jojo.png"

#define MUSIC_PATH "../assets/audio/music.mp3"
#define HOVER_SFX_PATH "../assets/audio/hover.wav"

// gameplay :
#define SQUARE_SIZE 100
#define MOVE_SPEED 10
#define FPS 120
#define CYCLE 32
#define MAX_ENEMIES 2

// Structures
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
    // function output
    SDL_Surface *surf;
    SDL_Rect rect;
} Text;

typedef struct {
    int h, w, x, y, x_center, y_center, type;
    SDL_Rect b_rect;
    SDL_Surface *basic;
    SDL_Surface *final;
    SDL_Rect rect;
    SDL_Surface *not_hovered;
    SDL_Surface *hovered;
    Text txt;
    int b_switch;
    int isHovered;
    int isPressed;
    int isClicked;
    int isSelected;
} Button;

typedef struct {
    SDL_Surface *bg;
    SDL_Color color;
    SDL_Surface *fill;
    Button *button;
    SDL_Rect rect;
    SDL_Rect s_rect;
    int val;
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
    SDL_Surface *bg;
    Button b;
    SDL_Rect rect;          // Input box rectangle
    Text txt;               // Text displayed inside
    int is_active;          // Whether the input box is active for typing
    char value[256];        // Stored text value
    int max_length;         // Maximum length of input
    int is_hovered;         // Hover state
    SDL_Surface *not_hovered; // Surface when not hovered
    SDL_Surface *hovered;     // Surface when hovered
    SDL_Surface *active;      // Surface when active
    int cursor_visible;       // Cursor visibility state
    Uint32 last_cursor_toggle; // Time of last cursor toggle
} My_input;

typedef struct {
    char jump[32];
    char left[32];
    char right[32];
    char up[32];
    char down[32];
    char dash[32]; // Added for dash ability
    char menu_up[32];
    char menu_down[32];
    char menu_select[32];
    char menu_escape[32];
} Controls;

typedef struct {
    Button *buttonlist;
    int selected_index;
    int b_ct;
    Text *txtlist;
    int txt_ct;
    SDL_Surface *background;
    int t_margine, b_margine;
    Slider *slider_list;
    int s_ct, s_margine;
    My_input *my_inputlist;
    int i_ct, i_margine;
} Menu;

typedef struct M_node {
    struct M_node *back;
    Menu *menu;
    int id;
    struct M_node *child_list;
} M_node;

typedef struct {
    SDLKey up;
    SDLKey down;
    SDLKey left;
    SDLKey right;
    SDLKey jump;
    SDLKey dash;
    float x_analog;
    float y_analog;
} Input;

typedef struct {
    SDL_Rect rect;
    SDL_Rect h_rect;
} Platform;

typedef struct {
    SDL_Color not_hovered_color ;
    SDL_Color hovered_color ;
    SDL_Color active_color ;
} InputStyle;

typedef struct {
    // movements
    int x, y;
    float x_speed, y_speed;
    float x_accel, y_accel;
    // stats
    int health;
    int level;
    // direction
    int move;
    int jump;
    int dash; // Added for dash ability
    int attack;
    int moveLeft, moveRight, moveUp, moveDown;
    // animation spritesheet
    SDL_Rect rect;
    SDL_Rect h_rect;
    SDL_Surface *surface;
    SDL_Surface *flying_to_the_right[8];
    SDL_Surface *flying_to_the_left[8];
    // second player sprite sheet
    SDL_Surface *p_flying_to_the_right[8];
    SDL_Surface *p_flying_to_the_left[8];
    int frame_counter;
    int index;
    int cycle;
    // fight animation
    int player_num;
    // input
    Input input;
    int jump_count;
    int max_jumps;
    int jump_trigger;
    // Dash-related fields
    int is_dashing;
    Uint32 dash_start_time;
    Uint32 dash_cooldown_end;
} Player;

typedef struct {
    int multiplayer;
    int serial_fd;
    char serial_buffer[256];
    int serial_buffer_len;
    Uint32 last_nav_time;
    ArduinoStatus arduino_status;
    ArduinoStatus prev_arduino_status;
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
    Player *player2;
    TTF_Font *mini_font;
    TTF_Font *main_font;
    TTF_Font *mid_font;
    TTF_Font *big_main_font;
    int x_mouse, y_mouse, x_button_size, y_button_size, margin;
    int released_mouse;
    int mouse_pressed;
    SDL_Surface *b_yellow;
    SDL_Surface *b_purple;
    int x_slider_size, y_slider_size;
    SDL_Color slider_fill_color;
    SDL_Color slider_bg_color;
    SDL_Surface *slider_bg, *slider_fill, *b_slider;
    SDL_Surface *background;
    SDL_Surface *black_surface;
    SDL_Surface *logo;
    Menu *current_menu;
    M_node *current_node;
    int escape;
    Mix_Chunk *sfx;
    Mix_Music *music;
    int music_volume;
    int sfx_volume;
    int frame;
    int cycle;
    int fps;
    int current_frame;
    time_t current_time;
    time_t last_time_update;
    char time_string[32];
    Uint32 last_fps_update;
    int frame_count;
    float current_fps;
    int selected_button_index;
    Uint32 last_joystick_nav;
    int joystick_nav_cooldown;
    Uint32 last_frame_time;
    int select;
    int controller_active;
    Uint32 last_mouse_motion;
    Platform *platforms;
    int platform_count;
    char player_name[256];
    Controls controls_p1;
    Controls controls_p2;
    int lvl_x_size;
    int lvl_y_size;
    int joystick_center_x; // Joystick X-axis center value
    int joystick_center_y;
    int controler_enabled;
} Game;

int gameplay(Game *game);
int gameplay2(Game *game);
int pizza();

#endif //GAME_H