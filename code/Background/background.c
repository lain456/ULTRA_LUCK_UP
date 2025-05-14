//
// Created by lain on 4/3/25.
//
#include "background.h"
#include "../tools/tools.h"


void load_background(Game *game) {
    SDL_Surface *background;



    int logo_w = 200,logo_h =140 ;

    //load
    background = IMG_Load(BACK_PNG_PATH);


    //scale
    background = scaleSurface(background,game->width, game->height);


    // no animation yet
    SDL_Rect logo_rect;
    logo_rect.w = logo_w;
    logo_rect.h = logo_h;
    logo_rect.x = game->width*1/20;
    logo_rect.y = game->height*4/5;




    // GHASSAN THIS IS YOUR JOB
    // load static images like this
    SDL_Surface *logo;
    SDL_BlitSurface(scaleSurface(IMG_Load(LOGO_PATH),logo_w, logo_h),NULL,background,&(SDL_Rect){logo_rect.x = game->width*1/20 ,game->height*12/20  +100, logo_w , logo_h});
    SDL_BlitSurface(scaleSurface(IMG_Load(LOGO2_PATH),logo_w * 4/5, logo_h *5/4),NULL,background,&(SDL_Rect){ game->width*1/20 + 20  ,game->height*14/20 +110  , logo_w , logo_h });


    // if you need to make stuff move around make your own struct and update thier posistion with a function



    game->background = background;


}





void render_background(Game *game)
{
    SDL_BlitSurface(game->background,NULL,game->screen,NULL);
}


