
#include "essentials.hpp"

#define FPS (data.secret & 0x01 ? 30 : 60)

int main(void) {
        SDL_Init(SDL_INIT_EVERYTHING);
        TTF_Init();

        GameData data;
        WinRend winrend;
        Assets assets(winrend.rend);

        std::cout << "\nlaunching...\n\n";
        intro(data.stars, winrend.rend, assets.font);

        std::cout <<"\nfinished intro...\n";

        Uint32 starting_tick = 0;
        bool quit = false;
        while(!quit) {
                starting_tick = SDL_GetTicks();

                quit = handler(data, winrend.win, assets.sounds);
                automata(data, assets.sounds);
                render(data, winrend.rend, assets);

                if(1000/FPS > SDL_GetTicks() - starting_tick)
                        SDL_Delay(1000/FPS  - (SDL_GetTicks() - starting_tick));
        }
        SDL_SetWindowFullscreen(winrend.win.get(), 0);

        data.leaderboard.write();

        std::cout << "exiting...\n";

        IMG_Quit();
        Mix_Quit();
        TTF_Quit();
        SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
        SDL_Quit();
}
