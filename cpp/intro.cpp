
#include "essentials.hpp"

#define INTRO_LENGTH_1 100U
#define INTRO_TIMER 15

const SDL_Rect mascot_rect = {(1280 - 300) / 2, 20, 300, 300};
const SDL_Rect studios_rect = {(1280 - 1000) / 2, 350, 1000, 125};
const SDL_Rect presents_rect = {(1280 - 600) / 2, 500, 600, 125};

static inline void render_stars(Star stars[MAX_STARS], std::shared_ptr<SDL_Renderer> rend) {
        for(size_t i = 0; i < MAX_STARS; ++i) {
                if(!is_on_screen(stars[i].rect)) continue;
                SDL_SetRenderDrawColor(rend.get(), stars[i].color.r, stars[i].color.g, stars[i].color.b, 255);
                SDL_RenderFillRect(rend.get(), &stars[i].rect);
        }
        SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 255);
}

int intro(Star stars[MAX_STARS], std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font) {
        std::cout << "starting intro...\n";

        SDL_Surface *surf = IMG_Load("../sprites/mascot.png");
        if(!surf) {
                std::cout << "couldn't find mascot\n";
                exit(EXIT_FAILURE);
        }
        SDL_Texture *mascot_text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);
        surf = TTF_RenderText_Blended(font.get(), "GELATIN STUDIOS", white);
        SDL_Texture *studios_text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);
        surf = TTF_RenderText_Blended(font.get(), "PRESENTS", white);
        SDL_Texture *presents_text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);
        Uint32 starting_tick = 0;
        Uint8 flag = 0;
        size_t i = 1;
        Uint8 timer = INTRO_TIMER;
        while(flag < 3) {
                starting_tick = SDL_GetTicks();

                SDL_Event event;
                while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                                SDL_DestroyTexture(mascot_text);
                                SDL_DestroyTexture(studios_text);
                                SDL_DestroyTexture(presents_text);
                                return 1;
                        } else if(event.type == SDL_KEYDOWN) goto SKIP;
                }

                SDL_SetTextureAlphaMod(mascot_text, i*i/INTRO_LENGTH_1);
                SDL_SetTextureAlphaMod(studios_text, i*i/INTRO_LENGTH_1);
                SDL_SetTextureAlphaMod(presents_text, i*i/INTRO_LENGTH_1);

                SDL_RenderClear(rend.get());
                render_stars(stars, rend);
                SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 255);
                SDL_RenderCopy(rend.get(), mascot_text, NULL, &mascot_rect);
                SDL_RenderCopy(rend.get(), studios_text, NULL, &studios_rect);
                SDL_RenderCopy(rend.get(), presents_text, NULL, &presents_rect);
                SDL_RenderPresent(rend.get());

                switch(flag) {
                        case 0:         ++i; break;
                        case 1:         --timer; break;
                        case 2:         --i; break;
                }

                if((!flag && i > INTRO_LENGTH_1 * 255 / i - 2) ||
                   (flag == 1 && !timer) || (flag == 2 && !i))
                         ++flag;


                if(1000/60 > SDL_GetTicks() - starting_tick)
                        SDL_Delay(1000/60  - (SDL_GetTicks() - starting_tick));

        }

SKIP:   SDL_DestroyTexture(mascot_text);
        SDL_DestroyTexture(studios_text);
        SDL_DestroyTexture(presents_text);

        surf = TTF_RenderText_Blended(font.get(), "SHMALAGA", white);
        SDL_Texture *title_text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);

        i = 0;
        flag = 0;
        timer = 100;
        while(timer) {
                starting_tick = SDL_GetTicks();

                SDL_Event event;
                while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || event.type == SDL_KEYDOWN) {
                                SDL_DestroyTexture(title_text);
                                return event.type != SDL_KEYDOWN;
                        }
                }

                const int width = i * 8;
                const int height = i * 2;
                const SDL_Rect title_rect = {(1280 - width) / 2, (720 - height) / 2, width, height};

                SDL_RenderClear(rend.get());
                render_stars(stars, rend);
                SDL_RenderCopy(rend.get(), title_text, NULL, &title_rect);
                SDL_RenderPresent(rend.get());

                if(!flag) ++i;
                else --timer;

                if(!flag && i >= 100) flag = 1;

                if(1000/60 > SDL_GetTicks() - starting_tick)
                        SDL_Delay(1000/60  - (SDL_GetTicks() - starting_tick));
        }

        SDL_DestroyTexture(title_text);

        return 0;
}
