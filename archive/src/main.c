#include "essentials.h"

//graphically intense
#define FPS (data.secret & 0x01 ? 30 : 60)
//#define FPS 30

int main(int argc, char *argv[]) {
        GameData data = {0};
        WinRend winrend = {0};
        Assets assets = {0};

        srand(time(NULL));

        SDL_Init(SDL_INIT_EVERYTHING);

        TTF_Init();

        Uint8 err = init_data(&data);
        if(err) {
                fprintf(stderr, "failed to initialize data\n\nterminating.\n\n");
                clean(&data, &winrend, &assets);
                puts("bruh moment");
                return EXIT_FAILURE;
        }

        err = init_winrend(&winrend);
        if(err) {
                fprintf(stderr, "failed to load window and renderer\n\nterminating.\n\n");
                clean(&data, &winrend, &assets);
                puts("bruh moment");
                return EXIT_FAILURE;
        }

        err = load_assets(winrend.rend, &assets);
        if(err) {
                fprintf(stderr, "failed to load assets\n\nterminating.\n\n");
                clean(&data, &winrend, &assets);
                puts("bruh moment");
                return EXIT_FAILURE;
        }

        Mix_Volume(-1, MIX_MAX_VOLUME/2);
        Mix_VolumeMusic(MIX_MAX_VOLUME/2);
        Mix_VolumeChunk(assets.sounds.explosion, MIX_MAX_VOLUME/8);

        puts("\nlaunching...\n");

        err = intro(data.stars, winrend.rend, assets.font);
        if(err) {
                fprintf(stderr, "failure with intro\n\nterminating.\n\n");
                clean(&data, &winrend, &assets);
                puts("bruh moment");
                return EXIT_FAILURE;
        }

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

        if(data.score.val > data.leaderboard[9].val) scores_update(data.leaderboard, data.score);

        print_scores(data.leaderboard);

        puts("exiting...");
        clean(&data, &winrend, &assets);

        // putchar('\n');
        // for(size_t i = 0; i < 10; ++i) {
        //         char name[4] = {0};
        //         for(size_t j = 0; j < 3; ++j)
        //                 name[j] = data.leaderboard[i].name[j];
        //         printf("%s: %u\n", name, data.leaderboard[i].val);
        // }
}
