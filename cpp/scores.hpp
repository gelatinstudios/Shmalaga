
#ifndef SCORES_H
#define SCORES_H

struct Score {
        char name[3] = {0};
        unsigned val = 0;
        void render_entry(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, SDL_Rect &rect, size_t n);
};

void name_handler(GameData &data, SDL_Event *event);
void render_name(GameData &, std::shared_ptr<SDL_Renderer>, std::shared_ptr<SDL_Texture>[], std::shared_ptr<TTF_Font>);

struct Leaderboard {
        Score scores[10];
        Leaderboard(void);
        void render(std::shared_ptr<SDL_Renderer>, std::shared_ptr<SDL_Texture>[], std::shared_ptr<TTF_Font>);
        void write(void);
        void update(Score);
};

#endif
