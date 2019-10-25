
#include "essentials.hpp"

static inline SDL_Texture *text_load(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, const char *str);
static inline SDL_Texture *sprite_load(std::shared_ptr<SDL_Renderer> rend, const char *img);
static inline SDL_Texture *create_sparkle(std::shared_ptr<SDL_Renderer> rend);

Assets::Assets(std::shared_ptr<SDL_Renderer> rend) {
        std::cout << "loading font...\n";
        font = std::shared_ptr<TTF_Font>(TTF_OpenFont("../press-start-2p.regular.ttf", 8), TTF_CloseFont);

        std::cout << "loading textures...\n";
        const char *menu_texts[] = {"MAIN MENU", "Sound", "Controls", "Quit", "Back"};
        for(size_t i = 0; i < LNGTH(textures.menu); ++i)
                textures.menu[i] = std::shared_ptr<SDL_Texture>(text_load(rend, font, menu_texts[i]), SDL_DestroyTexture);

        const char *texts[] = {"PAUSED", "PRESS ENTER", "sound muted", "YOU WIN", "YOU'RE DEAD", "Press ENTER",
                              "LIVES:", "YOU MADE THE LEADERBOARD!", "Press ENTER when you're done", "LEADERBOARD"};
        for(size_t i = 0; i < LNGTH(textures.texts); ++i)
                textures.texts[i] = std::shared_ptr<SDL_Texture>(text_load(rend, font, texts[i]), SDL_DestroyTexture);

        const char *filenames[] = {"spaceship.png", "enemy.png", "enemy1.png", "enemy2.png", "enemy3.png",
                                 "goldguy.png", "newboss.png", "good_bullet.png", "bad_bullet.png", "explosion.png"};
        for(size_t i = 0; i < LNGTH(textures.sprites); ++i)
                textures.sprites[i] = std::shared_ptr<SDL_Texture>(sprite_load(rend, filenames[i]), SDL_DestroyTexture);

        textures.sparkle = std::shared_ptr<SDL_Texture>(create_sparkle(rend), SDL_DestroyTexture);

        std::cout << "loading sounds...\n";
        Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
        sounds.music = std::shared_ptr<Mix_Music>(Mix_LoadMUS("../sounds/main_music.wav"), Mix_FreeMusic);
        if (!sounds.music) {
                std::cout << "couldn't load music\n";
                exit(EXIT_FAILURE);
        }
        sounds.explosion = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("../sounds/explosion.wav"), Mix_FreeChunk);
        if (!sounds.explosion) {
                std::cout << "couldn't load explosion.wav\n";
                exit(EXIT_FAILURE);
        }
        sounds.laser = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("../sounds/laser.wav"), Mix_FreeChunk);
        if (!sounds.laser) {
                std::cout << "couldn't load laser.wav\n";
                exit(EXIT_FAILURE);
        }
        sounds.bad_laser = std::shared_ptr<Mix_Chunk>(Mix_LoadWAV("../sounds/bad_laser.wav"), Mix_FreeChunk);
        if (!sounds.bad_laser) {
                std::cout << "couldn't load bad_laser.wav\n";
                exit(EXIT_FAILURE);
        }
}

Assets::~Assets(void) {
        std::cout << "destroying assets...\n";
}

static inline SDL_Texture *text_load(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, const char *str) {
        SDL_Surface *surf = TTF_RenderText_Blended(font.get(), str, white);

        SDL_Texture *text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);
        return text;
}

static inline SDL_Texture *sprite_load(std::shared_ptr<SDL_Renderer> rend, const char *img) {
        char filepath[50] = "../sprites/";
        strcat(filepath, img);
        SDL_Surface *surf = IMG_Load(filepath);
        if(!surf) {
                std::cout << "failed to load " << img << '\n';
                exit(EXIT_FAILURE);
        }

        SDL_Texture *text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);
        return text;
}

static inline SDL_Texture *create_sparkle(std::shared_ptr<SDL_Renderer> rend) {
        const SDL_Rect rects[] = {{0, 0, 1, 1}, {2, 0, 1, 1}, {1, 1, 1, 1}, {0, 2, 1, 1}, {2, 2, 1, 1}};
        SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, 3, 3, 8, SDL_PIXELFORMAT_RGBA32);
        SDL_FillRects(surf, rects, 5, SDL_MapRGB(surf->format, 255, 255, 255));
        SDL_Texture *text = SDL_CreateTextureFromSurface(rend.get(), surf);
        SDL_FreeSurface(surf);
        return text;
}
