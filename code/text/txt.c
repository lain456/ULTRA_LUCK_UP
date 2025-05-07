//
// Created by lain on 4/7/25.
//
#include "txt.h"









void update_txt(Text *txt, char *new_txt, SDL_Color new_color, TTF_Font *new_font)
{
    Text save = *txt;

    txt->color = new_color;
    if (new_font)
    {
        txt->font = new_font;
    }
    txt->writen = new_txt;

    // Free the old surface if it exists to prevent memory leaks
    if (txt->surf != NULL)
    {
        SDL_FreeSurface(txt->surf);
    }

    // Render the new text
    txt->surf = TTF_RenderText_Blended(txt->font, txt->writen, txt->color);
    if (txt->surf == NULL)
    {
        printf("Error rendering text: %s\n", TTF_GetError());
        return;
    }

    // Update rectangle with new dimensions
    txt->rect = (SDL_Rect){save.rect.x + ( save.rect.w - txt->surf->w)/2, save.rect.y + (save.rect.h - txt->surf->h )/2, txt->surf->w, txt->surf->h};
}






Text *create_txt(char *txt,TTF_Font *new_font,SDL_Color color,int x_pos,int y_pos)
{
    Text *res = (Text*)malloc(sizeof(Text));
    res->color  = color;
    res->writen = txt;
    res->surf = TTF_RenderText_Blended(new_font, txt, color);
    if (res->surf == NULL){printf("error loading txt surf ");}
    res->rect.w = res->surf->w;
    res->rect.h = res->surf->h;
    res->rect.x = x_pos;
    res->rect.y = y_pos;
    return res;

}


void render_txt(Game *game,Text *txt)
{
    SDL_BlitSurface(txt->surf, NULL, game->screen, &txt->rect);
}


void render_textlist(Game *game,Text *txtlist,int txt_ct)
{
    for (int i = 0; i < txt_ct; i++)
    {
        render_txt(game,&txtlist[i]);
    }
}



void x_center_txt(Game *game,Text *txt)
{
    txt->rect.x = (game->width - txt->surf->w)/2;
}


void y_center_txt(Game *game,Text *txt)
{
    txt->rect.y = (game->height - txt->surf->h)/2;
}