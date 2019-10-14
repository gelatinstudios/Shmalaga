#include "essentials.h"

static inline void render_entry(const Score *, SDL_Renderer *, TTF_Font *, SDL_Rect *, size_t);

void name_handler(GameData *data, SDL_Event *event) {
        switch(event->key.keysym.sym) {
                case SDLK_UP:           data->score.name[data->selected] = (data->score.name[data->selected] - 1 + 26 - 'A') % 26 + 'A'; break;
                case SDLK_DOWN:         data->score.name[data->selected] = (data->score.name[data->selected] + 1 - 'A') % 26 + 'A'; break;
                case SDLK_RIGHT:        data->selected = (data->selected + 1) % 3; break;
                case SDLK_LEFT:         data->selected = (data->selected - 1 + 3) % 3; break;
                case SDLK_RETURN:       data->selected = 0;
                                        data->gamestate = LEADERBOARD;
                                        //scores_update(data->leaderboard, data->score);
                                        break;
        }
}

void render_name(GameData *data, SDL_Renderer *rend, TTF_Font *font) {
        SDL_Rect rect = {1280/2 - 100/2 - 2*200, 720/2 - 300/2, 100, 300};

        for(size_t i = 0; i < 3; ++i) {
                char letter_str[] = {data->score.name[i], 0};
                SDL_Surface *surf;
                if(data->selected == i) surf = TTF_RenderText_Shaded(font, letter_str, black, white);
                else surf = TTF_RenderText_Blended(font, letter_str, white);
                rect.x += 200;
                render_surf(rend, surf, &rect);
        }
        SDL_RenderPresent(rend);
}

void render_leaderboard(Score scores[], SDL_Renderer *rend, TTF_Font *font) {
        SDL_Rect rect = {50, 100, 0, 50};

        for(size_t i = 0; i < 10; ++i) {
                if(i == 5) {
                        rect.x = 1280/2 + 50;
                        rect.y = 100;
                } else if(i == 9) rect.x -= 30;
                rect.y += 100;
                render_entry(&scores[i], rend, font, &rect, i);
        }

        SDL_RenderPresent(rend);
}

static inline void render_entry(const Score *score, SDL_Renderer *rend, TTF_Font *font, SDL_Rect *rect, size_t i) {
        char name[4] = {0};
        for(size_t i = 0; i < 3; ++i)
                name[i] = score->name[i] ? score->name[i] : '-';

        char str[30];
        sprintf(str, "%zu %s: %u", i+1, name, score->val);
        rect->w = strlen(str) * 30;
        SDL_Surface *surf = TTF_RenderText_Blended(font, str, white);
        render_surf(rend, surf, rect);
}
