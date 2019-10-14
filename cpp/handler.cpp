
#include "essentials.hpp"

#define SHIP_VEL 8
#define ONE_UP 100000U

static inline bool input_handler(GameData &, std::shared_ptr<SDL_Window>, Sounds &);
static inline void action_handler(GameData &, Sounds &);
static inline void player_death(GameData &, std::shared_ptr<Mix_Chunk>);

bool handler(GameData &data, std::shared_ptr<SDL_Window> win, Sounds &sounds) {
        bool quit = input_handler(data, win, sounds);
        action_handler(data, sounds);

        return quit;
}

static inline bool input_handler(GameData &data, std::shared_ptr<SDL_Window> win, Sounds &sounds) {
        bool quit = false;

        if(data.gamestate == IN_GAME && data.lives && (data.player_status == ALIVE || data.player_status == DEAD)) {
                if(data.bullets.bullet_timeout) --data.bullets.bullet_timeout;

                int vel = SHIP_VEL;
                if(SDL_GetModState() & KMOD_SHIFT) vel /= 2;

                const Uint8 *state = SDL_GetKeyboardState(NULL);
                if(state[data.keys[SHOOT_K]] && !data.bullets.bullet_timeout) {
                        data.bullets.create_gb(data.ship);
                        if(!data.muted) Mix_PlayChannel(-1, sounds.laser.get(), 0);
                        data.bullets.bullet_timeout = BULLET_TIMEOUT;
                }
                if(state[data.keys[LEFT_K]] && data.ship.x > 0) data.ship.x -= vel;
                if(state[data.keys[RIGHT_K]] && data.ship.x < 1280 - data.ship.w) data.ship.x += vel;
                if(state[data.keys[UP_K]] && data.ship.y > 0) data.ship.y -= vel;
                if(state[data.keys[DOWN_K]] && data.ship.y < 720 - data.ship.h) data.ship.y += vel;
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
                if(event.type == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT) quit |= true;
                else if(event.type != SDL_KEYDOWN) continue;

                const SDL_Keycode pressed = event.key.keysym.sym;

                if(data.gamestate != NAME_ENTRY) {
                        if(pressed == data.keys[MENU_K] && data.gamestate < MENU) {
                                data.gamestate = MENU;
                                break;
                        }
                        else if(pressed == data.keys[PAUSE_K]) data.gamestate = !data.gamestate;
                        else if(pressed == data.keys[MUTE_K]) data.muted = !data.muted;
                        else if(pressed == data.keys[FSCREEN_K]) {
                                data.gamestate = PAUSED;
                                SDL_SetWindowFullscreen(win.get(), data.fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
                                data.fullscreen = !data.fullscreen;
                        } else if(pressed == data.keys[RESET_K]) data.reset();
                        else if(pressed == SDLK_RETURN && data.gamestate == IN_GAME && !data.lives) {
                                if(data.score.val > data.leaderboard.scores[9].val) {
                                        data.gamestate = NAME_ENTRY;
                                        break;
                                } else {
                                        data.gamestate = LEADERBOARD;
                                        break;
                                }
                        }
                }

                if(data.gamestate == NAME_ENTRY) name_handler(data, &event);
                else if(data.gamestate == LEADERBOARD) {
                        if(pressed == SDLK_RETURN) data.reset();
                } else if(data.gamestate >= MENU) quit |= menu_handler(data, &event);
                else if(data.gamestate == STARTING_SCREEN) {
                        if(data.selected == 10) data.secret |= data.secret >> 4;

                        if(data.selected) {
                                if(data.selected < 10 && pressed == data.sekritptr[data.selected])
                                        ++data.selected;
                                else {
                                        data.secret &= 0x0F;
                                        data.selected = 0;
                                        data.sekritptr = NULL;
                                }
                        } else if(pressed == SDLK_RETURN) {
                                data.gamestate = IN_GAME;
                                Mix_PlayMusic(sounds.music.get(), -1);
                                data.selected = 0;
                                if(data.secret & 0x04) {
                                        data.ship.w = 12;
                                        data.ship.h = 12;
                                } else if(data.secret & 0x02) {
                                        data.ship.w = 100;
                                        data.ship.h = 100;
                                }
                                if(data.secret & 0x08) data.lives = 0xFF;
                        } else {
                                for(size_t i = 0; i < LNGTH(secret_codes); ++i) {
                                        if(pressed != secret_codes[i][0]) continue;

                                        data.sekritptr = secret_codes[i];
                                        data.secret |= (1 << i) << 4;
                                        data.selected = 1;
                                        break;
                                }
                        }
                }
        }


        return quit;
}

static inline void action_handler(GameData &data, Sounds &sounds) {
        if(data.gamestate != IN_GAME) return;

        //all enemies dead
        if(data.enemies.empty()) {
                ++data.level;
                data.load_level();
        }

        //player death sprite update
        if(data.player_status > ALIVE && data.player_status < DEAD) {
                if(!data.player_death_timeout) {
                        ++data.player_status;
                        if(data.player_status == DEAD && data.lives) {
                                 --data.lives;
                                 data.respawn_timeout = RESPAWN_TIMEOUT;
                        }
                        data.player_death_timeout = EXPLOSION_TIMEOUT;
                } else --data.player_death_timeout;
        }

        for(size_t i = 0; i < data.enemies.size(); ++i) {
                bool dead = data.enemies[i].handler(data, sounds.explosion);
                if(dead) data.enemies.erase(data.enemies.begin() + i);
        }

        data.gold_enemy.handler(data, sounds.explosion);

        //enemy bullet hits player
        for(auto &bb : data.bullets.bb)
                if(data.player_status == ALIVE && collision_detect(data.ship, MK_BB_RECT(bb.point)))
                        player_death(data, sounds.explosion);

        data.boss.handler(data, sounds.explosion);

        //respawn
        if(data.player_status == DEAD && data.lives) {
                if(!data.respawn_timeout) data.player_status = ALIVE;
                else --data.respawn_timeout;
        }

        if(data.score.val >= ONE_UP * (data.life_milestone+1)) {
                ++data.lives;
                ++data.life_milestone;
                data.one_up_timeout = ONE_UP_TIMEOUT;
        }

        //delete off-screen bullets
        if(!data.bullets.gb.empty() && data.bullets.gb[0].x >= 1280) data.bullets.gb.erase(data.bullets.gb.begin());
}

static inline void player_death(GameData &data, std::shared_ptr<Mix_Chunk> explosion) {
        data.player_status = DYING_1;
        data.player_death_timeout = EXPLOSION_TIMEOUT;
        if(!data.muted) Mix_PlayChannel(-1, explosion.get(), 0);
}

bool Enemy::handler(GameData &data, std::shared_ptr<Mix_Chunk> explosion) {
        bool dead = false;

        if(data.player_status == ALIVE && status == ALIVE && collision_detect(data.ship, rect))
                player_death(data, explosion);

        for(size_t i = 0; i < data.bullets.gb.size(); ++i) {
                if(status == ALIVE && collision_detect(MK_GB_RECT(data.bullets.gb[i]), rect)) {
                        status = DYING_1;
                        explosion_timeout = EXPLOSION_TIMEOUT;
                        data.bullets.gb.erase(data.bullets.gb.begin() + i);
                        if(!data.muted) Mix_PlayChannel(-1, explosion.get(), 0);
                        if(texture == SPR_GOLD_ENEMY) data.score.val += 2*ENEM_P;
                        else data.score.val += ENEM_P / pow(2, data.enemies[i].passes);
                }
        }
        if(data.gamestate == IN_GAME && status > ALIVE && status < DEAD) {
                if(!explosion_timeout) {
                        ++status;
                        if(status == DEAD) dead = true;
                        explosion_timeout = EXPLOSION_TIMEOUT;
                } else --explosion_timeout;
        }

        return dead;
}

void Boss::handler(GameData &data, std::shared_ptr<Mix_Chunk> explosion) {
        for(size_t i = 0; i < data.bullets.gb.size(); ++i) {
                if(status == ALIVE && collision_detect(MK_GB_RECT(data.bullets.gb[i]), rect)) {
                        --hp;
                        data.score.val += 5;
                        damage_timeout = DAMAGE_TIMEOUT;
                        data.bullets.bb.erase(data.bullets.bb.begin() + i);
                }
        }
        if(data.player_status == ALIVE && status == ALIVE && collision_detect(data.ship, rect))
                player_death(data, explosion);
        if(status == ALIVE && !hp) {
                status = DYING_1;
                //explosion_timeout = BOSS_EXPLOSION_TIMEOUT;
                new_exp_timeout = NEW_EXP_TIMEOUT;
                //if(!data.muted) Mix_PlayChannel(-1, sounds.explosion, 0);
        }
        if(status > ALIVE && status < DEAD) {
                if(new_exp_timeout) {
                        if(!(new_exp_timeout % NEW_EXP_SCALE)) {
                                const size_t i = (new_exp_timeout / NEW_EXP_SCALE) - 1;
                                explosions[i].rect.x = rng(rect.w - explosions[i].rect.w) + rect.x;
                                explosions[i].rect.y = rng(rect.h - explosions[i].rect.h) + rect.y;
                                explosions[i].timeout = EXPLOSION_TIMEOUT * 4;
                        }
                        --new_exp_timeout;
                        if(!new_exp_timeout) explosion_timeout = BOSS_EXPLOSION_TIMEOUT;
                } else if(explosion_timeout) {
                        --explosion_timeout;
                } else {
                        ++status;
                        explosion_timeout = BOSS_EXPLOSION_TIMEOUT;
                }

                for(size_t i = 0; i < NUM_EXP; ++i) {
                        if(explosions[i].timeout) --explosions[i].timeout;
                }
        }
}

void GameData::reset(void) {
        gamestate = IN_GAME;

        level = 1;

        score.val = 0;
        lives = 4;

        ship.x = 1280/4;
        ship.y = 720/2 - 56/2;
        player_status = ALIVE;

        bullets.gb.clear();
        enemies.clear();
        bullets.bb.clear();

        boss.rect.w = 500;
        boss.rect.h = 500;
        boss.status = DEAD;
        boss.hp = BOSS_HP;
        boss.pattern_timeout = 60 * 5;
        boss.angles[0] = 30;
        boss.angles[1] = 0;
        boss.angles[2] = 330;
        boss.angles[3] = 15;
        boss.angles[4] = 345;

        this->load_level();
}
