//
// Created by lain on 4/18/25.
//


#include "audio.h"
// learning what we need for audio manipulation ;

/*
 *if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    printf("SDL_mixer Error: %s\n", Mix_GetError());
    return 0;
}
*/


// int lib



/*
 *Mix_Music *music = Mix_LoadMUS(path) ;
 */

// load music;



/*
 *Mix_PlayMusic(music, -1);
 */



// playing continuous music





// setting volume
/*
*int volume = MIX_MAX_VOLUME / 2;
*Mix_VolumeMusic(volume);
*/


/*
 * ending shit
 *
*Mix_FreeMusic(music);
*Mix_CloseAudio();
*/


// things to discover : so what else do i need if i wanna play a lots of SFX and control thier vlolume and when to play and when to play another ?

// use wav format for sfx  :

//Mix_Chunk *sfx = Mix_LoadWAV("/home/lain/Desktop/nerding/SDL/LUCK UP/hover.wav");

// we load mix_chukns

//Mix_PlayChannel(-1, sfx, 0);


// how to change chunk vol ?


//Mix_VolumeChunk(sfx, MIX_MAX_VOLUME / 4);





Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);
    if (music == NULL) {
        printf("Mix_LoadMUS Error: %s\n", Mix_GetError());
    }
    return music;
}

