#include "essentials.h"

#define LEVEL 3

static inline char *title_generator(void);

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

int init_data(GameData *data) {
        puts("initializing data...");
        data->keys[UP_K] = SDL_SCANCODE_UP;
        data->keys[DOWN_K] = SDL_SCANCODE_DOWN;
        data->keys[LEFT_K] = SDL_SCANCODE_LEFT;
        data->keys[RIGHT_K] = SDL_SCANCODE_RIGHT;
        data->keys[SHOOT_K] = SDL_SCANCODE_Z;

        data->keys[MENU_K] = SDLK_ESCAPE;
        data->keys[PAUSE_K] = SDLK_p;
        data->keys[FSCREEN_K] = SDLK_f;
        data->keys[MUTE_K] = SDLK_m;
        data->keys[RESET_K] = SDLK_r;

        data->selected = 0;

        data->fullscreen = 0;
        data->muted = 0;

        data->gamestate = STARTING_SCREEN;
        data->level = LEVEL-1;
        data->level_timeout = LEVEL_TIMEOUT;

        load_scores(data->leaderboard);
        for(size_t i = 0; i < 3; ++i)
                data->score.name[i] = 'A';
        data->score.val = 0;

        data->lives = 4;
        //data->lives = 1;
        data->life_milestone = 0;

        data->stars = calloc(MAX_STARS, sizeof(Star));
        if(!data->stars) return 1;
        for(size_t i = 0; i < MAX_STARS; ++i) {
                data->stars[i].rect.x = rng(MAX_STAR_POS);
                data->stars[i].rect.y = rng(721);
                data->stars[i].rect.h = data->stars[i].rect.w = 1 + rng(4);

                Uint8 n = 1 + rng(100);
                if(n <= 80) { //white
                        data->stars[i].color.r = 255;
                        data->stars[i].color.g = 255;
                        data->stars[i].color.b = 255;
                } else if(n <= 86) { //red
                        data->stars[i].color.r = 255;
                        data->stars[i].color.g = 0;
                        data->stars[i].color.b = 0;
                } else if(n <= 93) { //blue
                        data->stars[i].color.r = 0;
                        data->stars[i].color.g = 0;
                        data->stars[i].color.b = 255;
                } else if(n <= 98) { //yellow
                        data->stars[i].color.r = 255;
                        data->stars[i].color.g = 255;
                        data->stars[i].color.b = 0;
                } else { //orange
                        data->stars[i].color.r = 255;
                        data->stars[i].color.g = 165;
                        data->stars[i].color.b = 0;
                }
        }

        for(size_t i = 0; i < 4; ++i) {
                data->clips.explosion[i].w = 32;
                data->clips.explosion[i].h = 32;
        }
        data->clips.explosion[0].x = 0;
        data->clips.explosion[0].y = 0;
        data->clips.explosion[1].x = 32;
        data->clips.explosion[1].y = 0;
        data->clips.explosion[2].x = 0;
        data->clips.explosion[2].y = 32;
        data->clips.explosion[3].x = 32;
        data->clips.explosion[3].y = 32;

        for(size_t i = 0; i < 4; ++i) {
                data->clips.gold_sprites[i].w = 20;
                data->clips.gold_sprites[i].h = 20;
        }
        data->clips.gold_sprites[0].x = 0;
        data->clips.gold_sprites[0].y = 0;
        data->clips.gold_sprites[1].x = 20;
        data->clips.gold_sprites[1].y = 0;
        data->clips.gold_sprites[2].x = 0;
        data->clips.gold_sprites[2].y = 20;
        data->clips.gold_sprites[3].x = 20;
        data->clips.gold_sprites[3].y = 20;

        data->ship.x = 1280/4;
        data->ship.y = 720/2 - 56/2;
        data->ship.h = 56;
        data->ship.w = 56;
        data->player_status = ALIVE;
        data->player_death_timeout = 0;

        //data->gb = calloc(MAX_BULLETS, sizeof(SDL_Rect));
        for(size_t i = 0; i < MAX_BULLETS; ++i) {
                data->gb[i].w = 10;
                data->gb[i].h = 10;
        }
        data->gb_count = 0;
        data->bullet_timeout = 0;

        data->enemies = calloc(MAX_ENEMIES, sizeof(Enemy));
        if(!data->enemies) return 1;
        for(size_t i = 0; i < MAX_ENEMIES; ++i) {
                data->enemies[i].rect.w = data->enemies[i].rect.h = 50 + rand()%16;
                data->enemies[i].status = DEAD;

                // this is all implicit
                // data->enemies[i].angle = 0;
                // data->enemies[i].circle = 0;
                // data->enemies[i].passes = 0;

                data->enemies[i].texture = rng(4) + SPR_ENEMY;
        }
        data->enemy_count = 0;
        data->living_enemies = 0;

        data->gold_enemy.rect.w = 55;
        data->gold_enemy.rect.h = 55;
        data->gold_enemy.status = DEAD;
        data->gold_enemy.texture = SPR_GOLD_ENEMY;

        data->boss.rect.w = 400;
        data->boss.rect.h = 400;
        data->boss.status = DEAD;
        data->boss.hp = 200;

        //data->bb = calloc(10000, sizeof(BB));
        //if(!data->bb) return 1;
        for(size_t i = 0; i < MAX_ENEMY_BULLETS; ++i) {
                data->bb[i].rect.w = 12;
                data->bb[i].rect.h = 8;
        }
        return 0;
}

int init_winrend(WinRend *winrend) {
        puts("initializing window and renderer...");
        winrend->win = SDL_CreateWindow(title_generator(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
        if(!winrend->win) return 1;
        //SDL_SetWindowFullscreen(winrend->win, SDL_WINDOW_FULLSCREEN);
        winrend->rend = SDL_CreateRenderer(winrend->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        return !winrend->rend;
}

static inline char *title_generator(void) {
        char* titles[] = {
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
                "\"Sorry I'm late! I was doing stuff\" \"I'm stuff\" \"MJ! OMG! NO!\" *laughing* \"Pete, your girlfriend is awesome\"",
                "Make sure you shoot as many bugs as you can! Bugs are icky!!!! ICKY ICKY ICKY!!!"
        };
        return titles[rng(LNGTH(titles))];
}
