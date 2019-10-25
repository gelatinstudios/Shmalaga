#ifndef ASSETS_H
#define ASSETS_H

enum {MN_TITLE, MN_SOUND, MN_CONTROLS, MN_QUIT, MN_BACK};
enum {TXT_PAUSED, TXT_START_SCREEN, TXT_MUTED, TXT_YOU_WIN, TXT_YOU_DIED, TXT_PRESSR, TXT_LIVES, TXT_MADE, TXT_ENTER_NAME, TXT_LDRBRD};
enum {SPR_SHIP, SPR_ENEMY, SPR_ENEMY1, SPR_ENEMY2, SPR_ENEMY3, SPR_GOLD_ENEMY, SPR_BOSS, SPR_GB, SPR_BB, SPR_EXPLOSION};
struct Textures {
        std::shared_ptr<SDL_Texture> menu[5];
        std::shared_ptr<SDL_Texture> texts[10];
        std::shared_ptr<SDL_Texture> sprites[10];
        std::shared_ptr<SDL_Texture> sparkle;
};

struct Sounds {
        std::shared_ptr<Mix_Music> music;
        std::shared_ptr<Mix_Chunk> explosion;
        std::shared_ptr<Mix_Chunk> laser;
        std::shared_ptr<Mix_Chunk> bad_laser;
};

struct Assets {
        std::shared_ptr<TTF_Font> font;
        Textures textures;
        Sounds sounds;
        Assets(std::shared_ptr<SDL_Renderer>);
        ~Assets(void);
};

#endif
