#include "essentials.h"

static inline Uint8 load_textures(SDL_Renderer *, TTF_Font *, Textures *);
static inline Uint8 text_load(SDL_Renderer *, TTF_Font *, SDL_Texture **, const char *);
static inline Uint8 text_load_b(SDL_Renderer *, TTF_Font *, SDL_Texture **, const char *);
static inline Uint8 sprite_load(SDL_Renderer *, SDL_Texture **, const char *);
static inline void load_sounds(Sounds *);
static inline void sound_load(Mix_Chunk **, const char *);

Uint8 load_assets(SDL_Renderer *rend, Assets *assets) {
        puts("loading font...");
        assets->font = TTF_OpenFont("prstart.ttf", 8);
        if(!assets->font) {
                fprintf(stderr, "error loading font:\n\t%s\n", TTF_GetError());
                return 1;
        }

        puts("loading textures...");
        Uint8 err = load_textures(rend, assets->font, &assets->textures);
        if(err) return 1;

        puts("loading sounds...");
        load_sounds(&assets->sounds);

        return 0;
}

static inline Uint8 load_textures(SDL_Renderer *rend, TTF_Font *font, Textures *textures) {
        Uint8 err = 0;

        err |= text_load(rend, font, &textures->menu.title, "MAIN MENU");
        err |= text_load(rend, font, &textures->menu.sound_w, "Sound");
        err |= text_load(rend, font, &textures->menu.controls_w, "Controls");
        err |= text_load(rend, font, &textures->menu.quit_w, "Quit");
        err |= text_load(rend, font, &textures->menu.back_w, "Back");

        err |= text_load_b(rend, font, &textures->menu.sound_b, "Sound");
        err |= text_load_b(rend, font, &textures->menu.controls_b, "Controls");
        err |= text_load_b(rend, font, &textures->menu.quit_b, "Quit");
        err |= text_load_b(rend, font, &textures->menu.back_b, "Back");

        err |= text_load(rend, font, &textures->texts.paused, "PAUSED");
        err |= text_load(rend, font, &textures->texts.start_screen, "PRESS ENTER");
        err |= text_load(rend, font, &textures->texts.muted, "sound muted");
        err |= text_load(rend, font, &textures->texts.you_win, "YOU WIN");
        err |= text_load(rend, font, &textures->texts.you_died, "YOU'RE DEAD");
        err |= text_load(rend, font, &textures->texts.pressr, "(press R)");
        err |= text_load(rend, font, &textures->texts.lives, "LIVES:");

        err |= sprite_load(rend, &textures->sprites.ship, "spaceship.png");
        err |= sprite_load(rend, &textures->sprites.enemy[0], "enemy.png");
        err |= sprite_load(rend, &textures->sprites.enemy[1], "enemy1.png");
        err |= sprite_load(rend, &textures->sprites.enemy[2], "enemy2.png");
        err |= sprite_load(rend, &textures->sprites.enemy[3], "enemy3.png");
        err |= sprite_load(rend, &textures->sprites.enemy[GOLD], "goldguy.png");
        err |= sprite_load(rend, &textures->sprites.gb, "good_bullet.png");
        err |= sprite_load(rend, &textures->sprites.bb, "bad_bullet.png");
        err |= sprite_load(rend, &textures->sprites.explosion, "explosion.png");

        return err;
}

static inline Uint8 text_load(SDL_Renderer *rend, TTF_Font *font, SDL_Texture **text, const char *str) {
        SDL_Surface *surf = TTF_RenderText_Blended(font, str, white);
        if(!surf) {
                fprintf(stderr, "error rendering white text\"%s\":\n\t%s\n", str, IMG_GetError());
                return 1;
        }

        *text = SDL_CreateTextureFromSurface(rend, surf);
        SDL_FreeSurface(surf);
        return 0;
}

static inline Uint8 text_load_b(SDL_Renderer *rend, TTF_Font *font, SDL_Texture **text, const char *str) {
        SDL_Surface *surf = TTF_RenderText_Shaded(font, str, black, white);
        if(!surf) {
                fprintf(stderr, "error rendering black text\"%s\":\n\t%s\n", str, IMG_GetError());
                return 1;
        }

        *text = SDL_CreateTextureFromSurface(rend, surf);
        SDL_FreeSurface(surf);
        return 0;
}

static inline Uint8 sprite_load(SDL_Renderer *rend, SDL_Texture **text, const char *img) {
        char filepath[25] = {0};
        sprintf(filepath, "sprites/%s", img);
        SDL_Surface *surf = IMG_Load(filepath);
        if(!surf) {
                fprintf(stderr, "error loading \"%s\":\n\t%s\n", img, IMG_GetError());
                return 1;
        }

        *text = SDL_CreateTextureFromSurface(rend, surf);
        SDL_FreeSurface(surf);
        return 0;
}

static inline void load_sounds(Sounds *sounds) {
        Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

        sounds->music = Mix_LoadMUS("sounds/music.wav");
        if(!sounds->music)
                fprintf(stderr, "error loading music:\n\t%s\n", Mix_GetError());

        sound_load(&sounds->explosion, "explosion.wav");
        sound_load(&sounds->laser, "laser.wav");
        sound_load(&sounds->bad_laser, "bad_laser.wav");
}

static inline void sound_load(Mix_Chunk **chunk, const char *file) {
        char filepath[25] = {0};
        sprintf(filepath, "sounds/%s", file);
        *chunk = Mix_LoadWAV(filepath);
        if(!*chunk)
                fprintf(stderr, "error loading \"%s\":\n\t%s\n", file, Mix_GetError());
}
