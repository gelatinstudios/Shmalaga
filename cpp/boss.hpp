#ifndef BOSS_H
#define BOSS_H

#define NUM_EXP 20

struct Explosion {
        SDL_Rect rect;
        Uint8 timeout;
};

class GameData;

struct Boss {
        SDL_Rect rect;
        Uint8 status;
        unsigned new_exp_timeout;
        unsigned explosion_timeout;
        unsigned angles[5];

        Explosion explosions[NUM_EXP];

        Uint8 pattern;
        Uint32 pattern_timeout;
        bool change;

        Uint8 bullet_timeout;
        Uint8 damage_timeout;
        unsigned hp;
        Boss(void);
        void handler(GameData &, std::shared_ptr<Mix_Chunk>);
        void render(std::shared_ptr<SDL_Renderer>, std::shared_ptr<SDL_Texture>[]);
        void boss_battle(GameData &, std::shared_ptr<Mix_Chunk>);
};

#endif
