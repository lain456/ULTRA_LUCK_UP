//
// Created by lain on 4/3/25.
//

#ifndef GAME_H
#define GAME_H
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include<math.h>



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
#define BACK_PNG_PATH "../assets/grphics/basic/back.png"
#define BUTTON_PNG_PATH "../assets/grphics/menu/white.png"
#define BUTTON_PNG2_PATH "../assets/grphics/menu/yellow.png"
//************************************************************************************//
//bird config
#define BIRD_R01_PATH "../assets/grphics/characters/bird/flying to the right/b2.png"
#define BIRD_R02_PATH "../assets/grphics/characters/bird/flying to the right/b2.png"
#define BIRD_R03_PATH "../assets/grphics/characters/bird/flying to the right/b3.png"
#define BIRD_R04_PATH "../assets/grphics/characters/bird/flying to the right/b4.png"
#define BIRD_R05_PATH "../assets/grphics/characters/bird/flying to the right/b5.png"
#define BIRD_R06_PATH "../assets/grphics/characters/bird/flying to the right/b6.png"
#define BIRD_R07_PATH "../assets/grphics/characters/bird/flying to the right/b7.png"
//************************************************************************************//
#define BIRD_L01_PATH "../assets/grphics/characters/bird/flying to the left/b1.png"
#define BIRD_L02_PATH "../assets/grphics/characters/bird/flying to the left/b2.png"
#define BIRD_L03_PATH "../assets/grphics/characters/bird/flying to the left/b3.png"
#define BIRD_L04_PATH "../assets/grphics/characters/bird/flying to the left/b4.png"
#define BIRD_L05_PATH "../assets/grphics/characters/bird/flying to the left/b5.png"
#define BIRD_L06_PATH "../assets/grphics/characters/bird/flying to the left/b6.png"
#define BIRD_L07_PATH "../assets/grphics/characters/bird/flying to the left/b7.png"

//************************************************************************************//
//font config :
#define FREDOKA_PATH "../assets/font/fredoka-one.one-regular.ttf"
//************************************************************************************//

#define LOGO_PATH "../assets/grphics/menu/main_logo.png"
#define LOGO2_PATH "../assets/grphics/menu/txt_logo.png"


#define MUSIC_PATH "../assets/audio/music.mp3"
#define HOVER_SFX_PATH "../assets/audio/hover.wav"


// gameplay :
#define SQUARE_SIZE 150
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
    SDL_Surface * not_hovered; ;
    SDL_Surface * hovered ;
    //text
    Text txt;

    //state
    int b_switch;
    int isHovered;
    int isPressed;
    int isClicked;
}Button;



typedef struct
{
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
    //SDL_Surface b_hovered;
    SDL_Rect b_rect;
    int is_clicked;
    int is_hovered;
    SDL_Color b_color;
    int x_dif;









} Slider;


typedef struct
{
    // menu elements :
    Button *buttonlist;
    int b_ct;
    Text *txtlist;

    int txt_ct;
    SDL_Surface *background;
    int t_margine,b_margine;


    Slider *slider_list;
    int s_ct,s_margine;
}Menu;



// creating nodes for our tree




typedef struct M_node
{
    struct M_node *back;
    Menu *menu;
    int id;
    // child_list size is included in the Menu->b_ct  AKA number of buttons
    struct M_node *child_list;
}M_node;



/*

typedef struct
{
    int x, y;
    float x_speed, y_speed;
    float x_accel, y_accel;
    //stats
    int health;
    int level; // Add level attribute
    // direction
    int move;



    int moveLeft,moveRight,moveUp,moveDown;

    // animation spritsheet WIP
    SDL_Surface *surface;
    SDL_Surface * flying_to_the_right[8];
    SDL_Surface * flying_to_the_left;


    // fight animation WIP

    int num;
}Bird;


*/






typedef struct
{
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
    //momvements
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

    // animation spritsheet WIP
    SDL_Surface *surface;
    SDL_Surface * flying_to_the_right[8];
    SDL_Surface * flying_to_the_left[8];
    int index;
    int cycle;

    // fight animation WIP
    int player_num;



    // input ...crap to make customizable
    Input input;

} Player;
















typedef struct {
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
    TTF_Font *mini_font;
    TTF_Font *main_font;
    TTF_Font * mid_font;
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
    SDL_Surface *slider_bg , *slider_fill  , *b_slider;






    //often used suf will be here :
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
    int current_frame;  // this number cycle each time it reaches the fps ( it goes from one to 60 if fps is 60)
    time_t current_time;

    // Time fields
    time_t last_time_update; // Last time the time string was updated
    char time_string[32];   // Buffer for formatted time (HH:MM:SS)

    // FPS fields
    Uint32 last_fps_update; // Last time FPS was calculated (SDL ticks)
    int frame_count;        // Number of frames since last FPS update
    float current_fps;







    // arduino
    ArduinoStatus arduino_status;



}Game;









int gameplay(Game *game);
int pizza() ;







#endif //GAME_H
