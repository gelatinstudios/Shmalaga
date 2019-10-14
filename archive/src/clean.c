#include "essentials.h"

#define DSTRY_TXT(text) { if(text) SDL_DestroyTexture(text); }
#define FREE_CHNK(chunk) { if(chunk) Mix_FreeChunk(chunk); }

void clean(GameData *data, WinRend *winrend, Assets *assets) {
        puts("freeing allocated data...");
        if(data->stars) free(data->stars);
        //if(data->gb) free(data->gb);
        if(data->enemies) free(data->enemies);
        //if(data->bb) free(data->bb);

        puts("destroying window and renderer...");
        if(winrend->win) SDL_DestroyWindow(winrend->win);
        if(winrend->rend) SDL_DestroyRenderer(winrend->rend);

        puts("freeing font...");;
        if(assets->font) TTF_CloseFont(assets->font);

        puts("destroying textures...");
        for(size_t i = 0; i < LNGTH(assets->textures.texts); ++i)
                DSTRY_TXT(assets->textures.texts[i]);
        for(size_t i = 0; i < LNGTH(assets->textures.menu); ++i)
                DSTRY_TXT(assets->textures.menu[i]);
        for(size_t i = 0; i < LNGTH(assets->textures.sprites); ++i)
                DSTRY_TXT(assets->textures.sprites[i]);

        puts("freeing sound assets...");
        if(assets->sounds.music) Mix_FreeMusic(assets->sounds.music);
        FREE_CHNK(assets->sounds.explosion);
        FREE_CHNK(assets->sounds.laser);
        FREE_CHNK(assets->sounds.bad_laser);

        puts("quitting SDL...");
        IMG_Quit();
        Mix_Quit();
        TTF_Quit();
        SDL_Quit();
}
