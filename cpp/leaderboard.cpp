
#include "essentials.hpp"

void name_handler(GameData &data, SDL_Event *event) {
        const char *keyname;
        switch(event->key.keysym.sym) {
                case SDLK_UP:           data.score.name[data.selected] = (data.score.name[data.selected] - 1 + 26 - 'A') % 26 + 'A'; break;
                case SDLK_DOWN:         data.score.name[data.selected] = (data.score.name[data.selected] + 1 - 'A') % 26 + 'A'; break;
                case SDLK_RIGHT:        data.selected = (data.selected + 1) % 3; break;
                case SDLK_LEFT:         data.selected = (data.selected - 1 + 3) % 3; break;
                case SDLK_RETURN:       data.selected = 0;
                                        data.leaderboard.update(data.score);
                                        data.gamestate = LEADERBOARD;
                                        break;

                default:                keyname = SDL_GetKeyName(event->key.keysym.sym);
                                        if(strlen(keyname) == 1 && isalpha(keyname[0]))
                                                data.score.name[data.selected] = keyname[0];
        }
}

void render_name(GameData &data, std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<SDL_Texture> texts[], std::shared_ptr<TTF_Font> font) {
        const SDL_Rect title = {(1280 - 1100) / 2, 50, 1100, 125};
        SDL_RenderCopy(rend.get(), texts[TXT_MADE].get(), NULL, &title);

        SDL_Rect rect = {(1280 - 75) / 2 - 2*150, (720 - 200) / 2, 75, 200};
        for(size_t i = 0; i < 3; ++i) {
                char letter_str[] = {data.score.name[i], 0};
                SDL_Surface *surf;
                if(data.selected == i) surf = TTF_RenderText_Blended(font.get(), letter_str, gold);
                else surf = TTF_RenderText_Blended(font.get(), letter_str, white);
                rect.x += 150;
                render_surf(rend, std::shared_ptr<SDL_Surface>(surf, SDL_FreeSurface), rect);
        }

        const SDL_Rect enter_name = {(1280 - 1000) / 2, 600, 1000, 75};
        SDL_RenderCopy(rend.get(), texts[TXT_ENTER_NAME].get(), NULL, &enter_name);

        SDL_RenderPresent(rend.get());
}


Leaderboard::Leaderboard(void) {
        std::ifstream file("scores", std::ifstream::in | std::ifstream::binary);
        file.read((char *) scores, sizeof scores);
        file.close();
}

void Score::render_entry(std::shared_ptr<SDL_Renderer>rend, std::shared_ptr<TTF_Font> font, SDL_Rect &rect, size_t n) {
        char name_buffer[4] = {0};
        if(!name[0])
                strcpy(name_buffer, "---");
        else {
                name_buffer[0] = name[0];
                name_buffer[1] = name[1];
                name_buffer[2] = name[2];
        }

        char str[30] = {0};
        sprintf(str, "%zu %s: %u", n+1, name_buffer, val);
        rect.w = strlen(str) * 30;
        std::shared_ptr<SDL_Surface> surf = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(font.get(), str, white), SDL_FreeSurface);
        render_surf(rend, surf, rect);
}

void Leaderboard::render(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<SDL_Texture> texts[], std::shared_ptr<TTF_Font> font) {
        const SDL_Rect title = {(1280 - 700) / 2, 50, 700, 100};
        SDL_RenderCopy(rend.get(), texts[TXT_LDRBRD].get(), NULL, &title);

        SDL_Rect rect = {50, 75, 0, 50};
        for(size_t i = 0; i < 10; ++i) {
                if(i == 5) {
                        rect.x = 1280/2 + 50;
                        rect.y = 75;
                } else if(i == 9) rect.x -= 30;
                rect.y += 100;
                scores[i].render_entry(rend, font, rect, i);
        }


        SDL_Rect press_enter_rect = {(1280 - 250) / 2, 650, 250, 50};
        SDL_RenderCopy(rend.get(), texts[TXT_PRESSR].get(), NULL, &press_enter_rect);

        SDL_RenderPresent(rend.get());
}

void Leaderboard::write(void) {
        std::ofstream file("scores", std::ofstream::out | std::ofstream::binary);
        file.write((const char *) scores, sizeof scores);
        file.close();
}

void Leaderboard::update(Score score) {
        size_t i = 8;
        while(score.val > scores[i].val && i < 9) {
                scores[i+1] = scores[i];
                --i;
        }
        scores[i+1] = score;
}
