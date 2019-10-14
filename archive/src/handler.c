#include "essentials.h"

#define SHIP_VEL 8
#define ONE_UP 250000U

static inline int input_handler(GameData *, SDL_Window *, Sounds *);
static inline void action_handler(GameData *, Sounds *);
static inline void enemy_handler(GameData *, Enemy *, Mix_Chunk *);
static inline void player_death(GameData *, Mix_Chunk *);
static inline void reset(GameData *);

int handler(GameData *data, SDL_Window *win, Sounds *sounds) {
        int quit = input_handler(data, win, sounds);
        action_handler(data, sounds);

        return quit;
}

static inline int input_handler(GameData *data, SDL_Window *win, Sounds *sounds) {
        int quit = 0;

        if(data->gamestate == IN_GAME && (data->player_status == ALIVE || data->player_status == DEAD)) {
                if(data->bullet_timeout) --data->bullet_timeout;

                const Uint8 *state = SDL_GetKeyboardState(NULL);
                if(state[data->keys[SHOOT_K]] && !data->bullet_timeout && data->player_status == ALIVE) create_gb(data, sounds);
                if(state[data->keys[LEFT_K]] && data->ship.x > 0) data->ship.x -= SHIP_VEL;
                if(state[data->keys[RIGHT_K]] && data->ship.x < 1280-56) data->ship.x += SHIP_VEL;
                if(state[data->keys[UP_K]] && data->ship.y > 0) data->ship.y -= SHIP_VEL;
                if(state[data->keys[DOWN_K]] && data->ship.y < 720-56) data->ship.y += SHIP_VEL;
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
                if(event.type == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT) quit |= 1;
                else if(event.type != SDL_KEYDOWN) continue;
                SDL_Keycode pressed = event.key.keysym.sym;
                if(pressed == data->keys[MENU_K] && data->gamestate < MENU) {
                        data->gamestate = MENU;
                        break;
                }
                else if(pressed == data->keys[PAUSE_K]) data->gamestate = !data->gamestate;
                else if(pressed == data->keys[MUTE_K]) data->muted = !data->muted;
                else if(pressed == data->keys[FSCREEN_K]) {
                        if(data->fullscreen) SDL_SetWindowFullscreen(win, 0); //passing 0 makes it a window again
                        else SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
                        data->fullscreen = !data->fullscreen;
                } else if(pressed == data->keys[RESET_K]) reset(data);
                else if(pressed == SDLK_RETURN && !data->lives) {
                        if(data->score.val > data->leaderboard[9].val) {
                                data->gamestate = NAME_ENTRY;
                                break;
                        } else {
                                data->gamestate = LEADERBOARD;
                                break;
                        }
                }

                if(data->gamestate == NAME_ENTRY) name_handler(data, &event);
                else if(data->gamestate == LEADERBOARD) {
                        if(pressed == SDLK_RETURN) reset(data);
                } else if(data->gamestate >= MENU) quit |= menu_handler(data, &event);
                else if(data->gamestate == STARTING_SCREEN) {
                        if(data->selected == 10) {
                                data->secret |= data->secret >> 4;
                                data->selected = 0;
                        } else if(data->selected) {
                                if(pressed == data->sekritptr[data->selected])
                                        ++data->selected;
                                else data->selected = 0;
                        } else if(pressed == SDLK_RETURN) {
                                data->gamestate = IN_GAME;
                                Mix_PlayMusic(sounds->music, -1);
                                data->selected = 0;
                                if(data->secret & 0x04) {
                                        data->ship.w = 12;
                                        data->ship.h = 12;
                                } else if(data->secret & 0x02) {
                                        data->ship.w = 100;
                                        data->ship.h = 100;
                                }
                                if(data->secret & 0x80) data->lives = 0xFF;
                        } else {
                                if(pressed == lsd_code[0]) {
                                        data->secret |= 0x10;
                                        data->sekritptr = lsd_code;
                                        data->selected = 1;
                                } else if(pressed == lives_code[0]) {
                                        data->secret |= 0x80;
                                        data->sekritptr = lives_code;
                                        data->selected = 1;
                                } else if(pressed == tiny_code[0]) {
                                        data->secret |= 0x40;
                                        data->sekritptr = tiny_code;
                                        data->selected = 1;
                                } else if(pressed == big_code[0]) {
                                        data->secret |= 0x20;
                                        data->sekritptr = big_code;
                                        data->selected = 1;
                                }
                        }
                }
        }


        return quit;
}

static inline void action_handler(GameData *data, Sounds *sounds) {
        //all enemies dead
        if(!data->living_enemies && data->level <= 7) {
                ++data->level;
                load_level(data);
        }

        //player death sprite update
        if(data->gamestate == IN_GAME && data->player_status > ALIVE && data->player_status < DEAD) {
                if(!data->player_death_timeout) {
                        ++data->player_status;
                        if(data->player_status == DEAD && data->lives) {
                                 --data->lives;
                                 data->respawn_timeout = RESPAWN_TIMEOUT;
                        }
                        data->player_death_timeout = EXPLOSION_TIMEOUT;
                } else --data->player_death_timeout;
        }

        for(size_t i = 0; i < data->enemy_count; ++i)
                enemy_handler(data, &data->enemies[i], sounds->explosion);

        enemy_handler(data, &data->gold_enemy, sounds->explosion);

        //enemy bullet hits player
        for(size_t i = 0; i < data->bb_count; ++i)
                if(data->player_status == ALIVE && collision_detect(data->ship, data->bb[i].rect))
                        player_death(data, sounds->explosion);

        //boss
        if(data->player_status == ALIVE && data->boss.status == ALIVE && collision_detect(data->ship, data->boss.rect))
                player_death(data, sounds->explosion);

        //respawn
        if(data->player_status == DEAD && data->lives) {
                if(!data->respawn_timeout) data->player_status = ALIVE;
                else --data->respawn_timeout;
        }

        if(data->score.val > ONE_UP * (data->life_milestone+1)) {
                ++data->lives;
                ++data->life_milestone;
                data->one_up_timeout = ONE_UP_TIMEOUT;
        }

        //delete off-screen bullets
        if(data->gb[0].x >= 1280 && data->gb_count) destroy_gb(data, 0);
}

static inline void player_death(GameData *data, Mix_Chunk *explosion) {
        data->player_status = DYING_1;
        data->player_death_timeout = EXPLOSION_TIMEOUT;
        if(!data->muted) Mix_PlayChannel(-1, explosion, 0);
}

static inline void enemy_handler(GameData *data, Enemy *enemy, Mix_Chunk *explosion) {
        if(data->player_status == ALIVE && enemy->status == ALIVE && collision_detect(data->ship, enemy->rect))
                player_death(data, explosion);

        for(size_t i = 0; i < data->gb_count; ++i) {
                if(enemy->status == ALIVE && collision_detect(data->gb[i], enemy->rect)) {
                        enemy->status = DYING_1;
                        enemy->explosion_timeout = EXPLOSION_TIMEOUT;
                        destroy_gb(data, i);
                        if(!data->muted) Mix_PlayChannel(-1, explosion, 0);
                        if(enemy->texture == SPR_GOLD_ENEMY) data->score.val += 2*ENEM_P;
                        else data->score.val += ENEM_P / pow(2, data->enemies[i].passes);
                }
        }
        if(data->gamestate == IN_GAME && enemy->status > ALIVE && enemy->status < DEAD) {
                if(!enemy->explosion_timeout) {
                        ++enemy->status;
                        if(enemy->status == DEAD && enemy->texture != SPR_GOLD_ENEMY) --data->living_enemies;
                        enemy->explosion_timeout = EXPLOSION_TIMEOUT;
                } else --enemy->explosion_timeout;
        }
}

static inline void reset(GameData *data) {
        data->gamestate = IN_GAME;

        data->level = 1;

        // if(data->score > load_mininum()) {
        //         print_score(data->score);
        //         data->highscore = load_highscore();
        // }
        data->score.val = 0;
        data->lives = 4;

        data->ship.x = 1280/4;
        data->ship.y = 720/2 - 56/2;
        data->player_status = ALIVE;

        while(data->gb_count) destroy_gb(data, 0);

        for(size_t i = 0; i < MAX_ENEMIES; ++i)
                data->enemies[i].angle = 0;

        while(data->bb_count) destroy_bb(data, 0);

        load_level(data);
}
