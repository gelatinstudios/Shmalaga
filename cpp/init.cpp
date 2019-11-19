
#include "essentials.hpp"

#define LEVEL 1
#define LIVES 4

static const char *title_generator(void);

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color gold = {255, 215, 0, 255};

const SDL_Keycode lsd_code[] = {
        SDLK_UP,
        SDLK_UP,
        SDLK_DOWN,
        SDLK_DOWN,
        SDLK_LEFT,
        SDLK_RIGHT,
        SDLK_LEFT,
        SDLK_RIGHT,
        SDLK_b,
        SDLK_a
};

const SDL_Keycode big_code[] = {
        SDLK_i,
        SDLK_a,
        SDLK_m,
        SDLK_a,
        SDLK_b,
        SDLK_i,
        SDLK_g,
        SDLK_b,
        SDLK_o,
        SDLK_y
};

const SDL_Keycode tiny_code[] = {
        SDLK_LEFT,
        SDLK_UP,
        SDLK_RIGHT,
        SDLK_DOWN,
        SDLK_LEFT,
        SDLK_UP,
        SDLK_t,
        SDLK_i,
        SDLK_n,
        SDLK_y
};

const SDL_Keycode lives_code[] = {
        SDLK_a,
        SDLK_b,
        SDLK_RIGHT,
        SDLK_LEFT,
        SDLK_RIGHT,
        SDLK_LEFT,
        SDLK_DOWN,
        SDLK_DOWN,
        SDLK_UP,
        SDLK_UP,
};

const SDL_Keycode *secret_codes[4] = {lsd_code, big_code, tiny_code, lives_code};

const SDL_Rect explosion_clips[4] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {0, 32, 32, 32},
        {32, 32, 32, 32}
};

const SDL_Rect boss_clips[4] = {
        {0, 0, 100, 100},
        {100, 0, 100, 100},
        {0, 100, 100, 100},
        {100, 100, 100, 100}
};

Enemy::Enemy(void) {
        rect.w = rect.h = 50 + rng(16);
        status = DEAD;
        angle = 0;
        circle = 0;
        passes = 0;
        texture = rng(4) + SPR_ENEMY;
}

Boss::Boss(void) {
        rect.w = 500;
        rect.h = 500;
        status = DEAD;
        hp = BOSS_HP;
        pattern_timeout = 60 * 5;
        angles[0] = 30;
        angles[1] = 0;
        angles[2] = 330;
        angles[3] = 15;
        angles[4] = 345;

        for(size_t i = 0; i < NUM_EXP; ++i)
                explosions[i].rect.w =
                explosions[i].rect.h = rng(30) + 70;
}

GameData::GameData(void) {
        std::cout << "initializing data...\n";
        keys[UP_K] = SDL_SCANCODE_UP;
        keys[DOWN_K] = SDL_SCANCODE_DOWN;
        keys[LEFT_K] = SDL_SCANCODE_LEFT;
        keys[RIGHT_K] = SDL_SCANCODE_RIGHT;
        keys[SHOOT_K] = SDL_SCANCODE_Z;

        keys[MENU_K] = SDLK_ESCAPE;
        keys[PAUSE_K] = SDLK_p;
        keys[FSCREEN_K] = SDLK_f;
        keys[MUTE_K] = SDLK_m;
        keys[RESET_K] = SDLK_r;
        selected = 0;

        fullscreen = false;
        muted = false;

        volumes.master = MIX_MAX_VOLUME / 2;
        volumes.music = MIX_MAX_VOLUME / 2;
        volumes.sfx = MIX_MAX_VOLUME / 2;

        gamestate = STARTING_SCREEN;
        level = LEVEL-1;
        level_timeout = LEVEL_TIMEOUT;

        score.name[0] = 'A';
        score.name[1] = 'A';
        score.name[2] = 'A';
        score.val = 0;

        lives = LIVES;
        life_milestone = 0;

        //stars = calloc(MAX_STARS, sizeof(Star));
        //if(!stars) return 1;
        for(size_t i = 0; i < MAX_STARS; ++i) {
                stars[i].rect.x = rng(MAX_STAR_POS);
                stars[i].rect.y = rng(721);
                stars[i].rect.h = stars[i].rect.w = 1 + rng(4);

                Uint8 n = 1 + rng(100);
                if(n <= 80) { //white
                        stars[i].color.r = 255;
                        stars[i].color.g = 255;
                        stars[i].color.b = 255;
                } else if(n <= 86) { //red
                        stars[i].color.r = 255;
                        stars[i].color.g = 0;
                        stars[i].color.b = 0;
                } else if(n <= 93) { //blue
                        stars[i].color.r = 0;
                        stars[i].color.g = 0;
                        stars[i].color.b = 255;
                } else if(n <= 98) { //yellow
                        stars[i].color.r = 255;
                        stars[i].color.g = 255;
                        stars[i].color.b = 0;
                } else { //orange
                        stars[i].color.r = 255;
                        stars[i].color.g = 165;
                        stars[i].color.b = 0;
                }
        }

        ship.x = 1280 / 4;
        ship.y = (720 - 56) / 2;
        ship.h = 56;
        ship.w = 56;
        player_status = ALIVE;
        player_death_timeout = 0;

        gold_enemy.rect.w = 55;
        gold_enemy.rect.h = 55;
        gold_enemy.status = DEAD;
        gold_enemy.texture = SPR_GOLD_ENEMY;
}


WinRend::WinRend(void) {
        std::cout << "initializing window and renderer...\n";

        if (SDL_GetNumVideoDisplays() < 1) return 1;
        SDL_Rect display_bounds;
        SDL_GetDisplayBounds(0, &display_bounds);
        const int w = (3 * display_bounds.w / 4);
        win = std::shared_ptr<SDL_Window>(SDL_CreateWindow(title_generator(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, 9 * w / 16, 0), SDL_DestroyWindow);
        rend = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(&*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);
        SDL_RenderSetLogicalSize(rend.get(), 1280, 720);
        SDL_ShowCursor(SDL_DISABLE);

        SDL_Surface *icon = IMG_Load("sprites/mascot.png");
        SDL_SetWindowIcon(win.get(), icon);
        SDL_FreeSurface(icon);
}

WinRend::~WinRend(void) {
        std::cout << "destroying window and renderer...\n";
}

static const char *title_generator(void) {
        const char *titles[] = {
                "Shmalaga - The hackable Galaga clone for the 21st century",
                "Tell all your friends, family, and co-workers about Shmalaga!",
                "Tell everyone with which you come into contact for the next 2 weeks about Shmalaga or face the consequences!!",
                "[REDACTED]",
                "You'll wish you never played it!",
                "That land belongs to the Palestinians!",
                "what? what's happening? where am i? what is this? oh god...oh my god... no... no... NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO",
                "ET ME OUT OF HERE GET ME OUT OF HERE GET ME OUT OF HERE GET ME OUT OF HERE GET ME OUT OF HERE GET ME OUT OF HERE GE",
                "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
                "Enjoy a delicious McChicken from your nearest McDonald's, today!",
                "Shmagala",
                "You ever watch Kill La Kill? That show is really good.",
                "Don't make me say the G-word",
                "Sitting on this barstool talking like a damn fool",
                "Barack Obama",
                "A man walks into a doctor's office and says, \"Doctor, it hurts when I do this.\" The doctor replies, \"Don't do that.\"",
                "sus",
                "This game is VERY GRAPHICALLY INTENSE and if your cheap laptop can't run it at 60 fps then GET A JOB LMAO",
                "Enjoy a delicios McChicken from your nearest McChick-fil-a, today!",
                "Enjoy a delicious kkkkkmmmmmm from your nearest pllllllllllllll, tomorrow!",
                "enjoy sus",
                "Enjoy any two-topping ice cream cone from Uncle Caldwell's Crazy Cream Conery with a 5% discount all throughout this weekend!",
                "Enjoy a delicious calzone from Big Pappa's Big 'Ole Pizzeria Eatery Bakery, today!",
                "Enjoy a chicken wing from Ol' Missie's Chickin Pickin' Dinin' and Dickin' Chicken-Based Diner and Cuisine, today!",
                "Enjoy a knuckle-sandwich from Ruford, your old school bully, today!",
                "Enjoy Shmalaga! Enjoy Shmalaga! Enjoy Shmalaga! Enjoy Shmalaga! Enjoy Shmalaga! Enjoy Shmalaga!",
                "Want to stop and shop for a delicious Egg McMuffin? Make a left turn at the next traffic light!",
                "Cruisin' for a snoozin'? Try overdosing on Xanax, today!",
                "Stage 3 is the worst!",
                "\"Sorry I'm late! I was doing stuff\" \"I'm stuff\" \"MJ! OMG! NO!\" *laughing* \"Pete, your girlfriend is awesome\"",
                "Make sure you shoot as many bugs as you can! Bugs are icky!!!! ICKY ICKY ICKY!!!",
                "My life is like a video game. Trying hard to make the grade. All while I am still collecting coins."
        };
        return titles[rng(LNGTH(titles))];
}
