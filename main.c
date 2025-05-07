#include "init/init.h"
#include "game/game.h"
#include "load_menus/load_menus.h"


#include "tools/tools.h"
#include "Player/player.h"
#include"Background/background.h"
#include "button/button.h"

#include "slider/slider.h"
#include "audio/audio.h"
#include "text/txt.h"
#include "menu/menu.h"
































































int main(int argc, char *argv[]){
    printf("pizza = %d\n",pizza());
    Game game;
    Ini_Game(&game);
    load_background(&game);
    game.music = loadMusic(MUSIC_PATH);
    game.sfx = Mix_LoadWAV(HOVER_SFX_PATH);
    Mix_PlayMusic(game.music, -1);
    printf("testing ... \n");
    // obv testing ....
    //return test();
    //return test3();
    //return test4();

    //return test5();
    //return fat_ass_audio_test();
    //return test5(&game);
    //return gameplay(&game);













    //init phase...



    M_node n;
    M_node n1;
    M_node n0;
    M_node n2;


    Menu play = play_menu(game); ;
    Menu exit = exit_menu(game);
    Menu options = options_menu(game);
    Menu GET_YO_SORRY_ASS_TO_WORK = WIP_menu(game);
    node_Init(&n0,&exit,0 );
    node_Init(&n1,&play,1);
    node_Init(&n2,&options,2);
    node_Init(&n,&GET_YO_SORRY_ASS_TO_WORK,-1);



    n0.back = &n1; // get back to main
    n1.back = &n0; // get to exit
    n2.back = &n1;
    n.back = &n1;









    //WIP


    printf("%d\n",n0.back->id);

    //n1.parent = &n0;

    game.current_node = &n1;



    //node_Init(&exit,play_menu(game));

    // WIP exit



    //cooked = exit_menu(game);


   // main_menu.parent = &exit;




    game.current_menu = n1.menu;

    //SDL_Event event;   // use the game event
    while (!game.quite) {
        SDL_GetMouseState(&game.x_mouse,&game.y_mouse);
        game.released_mouse = 0;
        game.mouse_pressed =0;
        while (SDL_PollEvent(&game.event)) {
            switch (game.event.type) {
            case SDL_KEYDOWN: // Key pressed
                if (game.event.key.keysym.sym == SDLK_ESCAPE && game.state ==1) {
                    game.current_node = game.current_node->back;
                }
                break;
            case SDL_QUIT: // Window close button
                game.quite = 1;
                break;
            }
            if (game.event.type == SDL_MOUSEBUTTONDOWN) {
                game.mouse_pressed = 1;
                //printf("Mouse button pressed %d\n", game.event.button.button);
            }
            if (game.event.type == SDL_MOUSEBUTTONUP) {
                //game.mouse_pressed = 0;
                //printf("Mouse button released %d\n", game.event.button.button);
                game.released_mouse = game.event.button.button;
                game.event.button.button = 0;
            }
        }



        // the big switch
        switch (game.state)
        {

            case 1:{



            update_buttons(&game,game.current_node->menu->buttonlist,game.current_node->menu->b_ct);



            switch (game.current_node->id)
            {

            case -1:
                {
                    if (game.current_node->menu-> buttonlist[0].isClicked )
                    {
                        game.current_node = &n1;
                    }
                    break;
                }


            case 0 :
                {
                    if (game.current_node->menu-> buttonlist[0].isClicked )
                    {
                        game.current_node = &n1;

                    }


                    if (game.current_node->menu-> buttonlist[1].isClicked )
                    {
                        game.quite = 1;
                    }


                    break;
                }
            case 1:
                {




                    if (game.current_node->menu-> buttonlist[0].isClicked )
                    {
                        /*
                        update_txt(&game.current_node->menu->buttonlist[0].txt,"P L A Y",GOLD,NULL);
                        printf("we are about to play\n");
                        */
                        game.state = 0;
                        break;

                    }


                    // options
                    if (game.current_node->menu-> buttonlist[1].isClicked ){
                        game.current_node = &n2;
                        break; // big fix
                    }


                    // info

                    if (game.current_node->menu-> buttonlist[2].isClicked ){
                        game.current_node = &n;
                        break;
                    }




                    // quite
                    if (game.current_node->menu-> buttonlist[3].isClicked ){
                        game.current_node = &n0;
                        break;
                    }

                    // HELP
                    if (game.current_node->menu-> buttonlist[4].isClicked ){
                        game.current_node = &n;
                        break;
                    }





                    break;
                }

            case 2:
                {

                    // auto  on / off music



                    if (game.current_node->menu-> buttonlist[0].isClicked )
                    {
                        update_txt(&game.current_node->menu->txtlist[0] ,". . . ",BLACK,game.big_main_font);

                        // fixed a bug here     -lain
                        switch (game.music_volume )
                        {
                        case 0:
                            {
                                game.music_volume = 69;
                                break;
                            }
                        default:
                            {
                                game.music_volume = 0;
                                break;
                            }
                        }
                    }









                    // auto  on / off sfx



                    if (game.current_node->menu-> buttonlist[1].isClicked )
                    {
                        //update_txt(&game.current_node->menu->txtlist[1] ,"yeah that button doesn't work...yet",BLACK,game.main_font);
                        printf("gp %d , gr %d , mv %d sfv %d\n",game.mouse_pressed,game.released_mouse,game.music_volume,game.sfx_volume);

                        // fixed a bug here     -lain
                        switch (game.sfx_volume )
                        {
                        case 0:
                            {
                                game.sfx_volume = 69;
                                break;
                            }
                        default:
                            {
                                game.sfx_volume = 0;
                                break;
                            }
                        }
                        break;
                    }



                    update_slider(&game,&game.current_node->menu->slider_list[0],game.music_volume);
                    update_slider(&game,&game.current_node->menu->slider_list[1],game.sfx_volume);
                    //the vol is being updated
                    game.music_volume = game.current_node->menu->slider_list[0].val;
                    game.sfx_volume = game.current_node->menu->slider_list[1].val;











                    // txt change
                    if (game.music_volume!=0)
                    {
                        update_txt(&game.current_node->menu->buttonlist[0].txt,"music on",GOLD,NULL);
                        //update_txt(&game.current_node->menu->txtlist[0] ," :D  ",BLACK,game.big_main_font);

                    }else
                    {
                        update_txt(&game.current_node->menu->buttonlist[0].txt,"music off",GOLD,NULL);

                    }

                    if (game.sfx_volume!=0)
                    {
                        update_txt(&game.current_node->menu->buttonlist[1].txt,"sfx : on",GOLD,NULL);

                    }else
                    {
                        update_txt(&game.current_node->menu->buttonlist[1].txt,"sfx : off",GOLD,NULL);
                        //update_txt(&game.current_node->menu->txtlist[0] ," no sfx ?",BLACK,game.big_main_font);

                    }








                    switch (game.fullscreen)
                    {
                    case 0:
                        {
                            update_txt(&game.current_node->menu->buttonlist[2].txt,"fullscreen on",GOLD,game.mini_font);
                            break;
                        }
                    default:
                        {
                            update_txt(&game.current_node->menu->buttonlist[2].txt,"fullscreen off",GOLD,game.mini_font);
                            break;
                        }

                    }




                    if (game.current_node->menu-> buttonlist[2].isClicked )
                    {
                        toggle_fullscreen(&game) ;

                        break;
                    }




                    if (game.current_node->menu-> buttonlist[3].isClicked )
                    {
                        update_txt(&game.current_node->menu->txtlist[0] ," welcome back :D",BLACK,game.big_main_font);
                        game.current_node = &n1;
                        break;
                    }




                    break;
                }





            default:
                printf("get out ");
                break;
            }



















            render_background(&game);
            render_menu(&game,game.current_node->menu);







            break;
            }
            case 0:
                {
                    //test5(&game);
                    gameplay(&game);

                    break;
                }
        }

        SDL_Flip(game.screen);
        Mix_VolumeMusic( MIX_MAX_VOLUME *  game.music_volume/100 );
        Mix_VolumeChunk(game.sfx, MIX_MAX_VOLUME * game.sfx_volume/100);
        //printf("gp %d , gr %d , mv %d\n",game.mouse_pressed,game.released_mouse,game.music_volume);
    }




    SDL_Quit();
    return 0;

}
