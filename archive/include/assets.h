#ifndef ASSETS_H
#define ASSETS_H

enum {MN_TITLE, MN_SOUND, MN_CONTROLS, MN_QUIT, MN_BACK};
enum {TXT_PAUSED, TXT_START_SCREEN, TXT_MUTED, TXT_YOU_WIN, TXT_YOU_DIED, TXT_PRESSR, TXT_LIVES};
enum {SPR_SHIP, SPR_ENEMY, SPR_ENEMY1, SPR_ENEMY2, SPR_ENEMY3, SPR_GOLD_ENEMY, SPR_BOSS, SPR_GB, SPR_BB, SPR_EXPLOSION};
typedef struct {
        SDL_Texture *menu[5];
        SDL_Texture *texts[7];
        SDL_Texture *sprites[10];
} Textures;

typedef struct {
        Mix_Music *music;
        Mix_Chunk *explosion;
        Mix_Chunk *laser;
        Mix_Chunk *bad_laser;
} Sounds;

typedef struct {
        TTF_Font *font;
        Textures textures;
        Sounds sounds;
} Assets;

int load_assets(SDL_Renderer *, Assets *);

#endif
