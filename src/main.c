#include "essentials.h"

//graphically intense
#define FPS (data.secret & 0x01 ? 30 : 60)
//#define FPS 30

static inline void error(const char *, WinRend *, Assets *);

int main(int argc, char *argv[]) {
        GameData data = {0};
        WinRend winrend = {0};
        Assets assets = {0};

        srand(time(NULL));

        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();
        Mix_Init(MIX_INIT_OGG);

        Uint8 err = init_data(&data);
        if(err) error("initialize data", &winrend, &assets);

        err = init_winrend(&winrend);
        if(err) error("load window and renderer", &winrend, &assets);

        err = load_assets(winrend.rend, &assets);
        if(err) error("load assets", &winrend, &assets);

        Mix_Volume(-1, MIX_MAX_VOLUME/2);
        Mix_VolumeMusic(MIX_MAX_VOLUME/2);
        //Mix_VolumeChunk(assets.sounds.explosion, MIX_MAX_VOLUME/8);

        puts("\nlaunching...\n");

        err = intro(data.stars, winrend.rend, assets.font);
        if(err) error("render intro", &winrend, &assets);

        Uint32 starting_tick = 0;
        Uint8 quit = 0;
        while(!quit) {
                starting_tick = SDL_GetTicks();

                quit = handler(&data, winrend.win, &assets.sounds);
                automata(&data, &assets.sounds);
                render(&data, winrend.rend, &assets);

                if(1000/FPS > SDL_GetTicks() - starting_tick)
                        SDL_Delay(1000/FPS  - (SDL_GetTicks() - starting_tick));
        }
        SDL_SetWindowFullscreen(winrend.win, 0);

        print_scores(data.leaderboard);

        puts("exiting...");
        clean(&winrend, &assets);
}

static inline void error(const char *str, WinRend *winrend, Assets *assets) {
        fprintf(stderr, "failed to %s\n\nterminating.\n\n", str);
        clean(winrend, assets);
        puts("bruh moment");
        exit(EXIT_FAILURE);
}
