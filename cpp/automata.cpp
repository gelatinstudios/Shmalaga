
#include "essentials.hpp"

#define STAR_VEL 8
#define ENEMY_VEL 6
#define SQRT3 1.732050807568877

static inline void move_enemies(GameData &, Sounds &);

void automata(GameData &data, Sounds &sounds) {
        if(data.gamestate == IN_GAME || data.gamestate == NAME_ENTRY || data.gamestate == LEADERBOARD) {
                for(auto &star : data.stars) {
                        star.rect.x -= STAR_VEL;
                        if(star.rect.x < 0) star.rect.x = MAX_STAR_POS;
                }
        }
        if(data.gamestate == IN_GAME) {
                if(data.level < 7) move_enemies(data, sounds);
                else data.boss.boss_battle(data, sounds.bad_laser);

                for(auto &gb : data.bullets.gb)
                        gb.x += BULLET_VEL;

                for(auto &bb : data.bullets.bb) {
                        bb.point.x -= rint(bb.dx);
                        bb.point.y -= rint(bb.dy);
                }

                while(!data.bullets.bb.empty() && !is_on_screen(MK_BB_RECT(data.bullets.bb[0].point)))
                        data.bullets.bb.erase(data.bullets.bb.begin());
        }
}

static inline void move_enemies(GameData &data, Sounds &sounds) {
        for(auto &enemy : data.enemies) {
                if(enemy.status == DEAD) continue;
                else if(enemy.status > ALIVE && enemy.status < DEAD) {
                        enemy.rect.x -= ENEMY_VEL;
                        continue;
                }

                switch(data.level) {
                        case 1:
                        case 2:         enemy.rect.x -= ENEMY_VEL;
                                        if(enemy.rect.x < 0 - enemy.rect.w) {
                                                enemy.rect.x = 1280 * (data.level == 1 ? 2 : 6);
                                                enemy.passes;
                                        }
                                        break;
                        case 3:         enemy.lvl3(); break;
                        case 4:         enemy.lvl4(); break;
                        case 5:         enemy.lvl5(data.ship); break;
                        case 6:         enemy.lvl6(data.ship, data.player_status == ALIVE); break;
                }
                if(is_on_screen(enemy.rect) && !rng(800)) {
                        data.bullets.create_bb(enemy.rect, enemy.angle);
                        if(!data.muted) Mix_PlayChannel(-1, sounds.bad_laser.get(), 0);
                }
        }

        if(data.gold_enemy.status == ALIVE) {
                data.gold_enemy.rect.x -= 1.5 * ENEMY_VEL;
                data.sparkle.rect.x -= 1.5 * ENEMY_VEL;
        }
}

void Enemy::lvl3(void) {
        if(!flag) {
                if(rect.y > 720) {
                        rect.x += (720+60)/SQRT3;
                        rect.y = -24*200 + 50;
                        ++passes;
                }

                if(rect.y < -60) rect.y += ENEMY_VEL;
                else {
                        rect.x -= ENEMY_VEL/2;
                        rect.y += SQRT3*ENEMY_VEL/2.0;
                }
        } else {
                if(rect.y < -60) {
                        rect.x += (720+60)/SQRT3;
                        rect.y = 1280*2-60 + 24*200 - 200 + 50;
                        ++passes;
                }

                if(rect.y > 720) rect.y -= ENEMY_VEL;
                else {
                        rect.x -= ENEMY_VEL/2;
                        rect.y -= SQRT3*ENEMY_VEL/2.0;
                }
        }
}

void Enemy::lvl4(void) {
        if(!flag) {
                if(!circle && rect.y > rect.h && (!rng(75) || rect.y > 460))
                        circle = true;

                if(rect.y < 0 - rect.h) rect.y += ENEMY_VEL;
                else if(!circle) {
                        rect.x += ENEMY_VEL/2;
                        rect.y += SQRT3*ENEMY_VEL/2.0;
                } else this->mov_circ();
        } else {
                if(!circle && rect.y < 720-60-60 && (!rng(75) || rect.y < 120))
                        circle = true;

                if(rect.y > 720) rect.y -= ENEMY_VEL;
                else if(!circle) {
                        rect.x += ENEMY_VEL/2;
                        rect.y -= SQRT3*ENEMY_VEL/2.0;
                } else this->mov_circ_ccwise();
        }
}

void Enemy::mov_circ(void) {
        rect.x -= rint(ENEMY_VEL * cos(DEGRAD(angle)));
        rect.y -= rint(ENEMY_VEL * sin(DEGRAD(angle)));
        angle = ((int) angle + 3) % 360;
        if(angle == 240) ++passes;
}

void Enemy::mov_circ_ccwise(void) {
        rect.x -= rint(ENEMY_VEL * cos(DEGRAD(angle)));
        rect.y -= rint(ENEMY_VEL * sin(DEGRAD(angle)));
        angle = ((int) angle - 3 + 360) % 360;
        if(angle == 120) ++passes;
}

void Enemy::lvl5(const SDL_Rect ship) {
        if(rect.x < 0 - rect.w) {
                rect.x = 1280*7;
                ++passes;
        }
        if(rect.x > 1000) rect.x -= ENEMY_VEL;
        else {
                rect.x -= rint(2.0*ENEMY_VEL * cos(DEGRAD(angle)));
                rect.y -= rint(2.0*ENEMY_VEL * sin(DEGRAD(angle)));
                angle = RADDEG(atan( ((double) (rect.y - ship.y)) / ((double) abs(rect.x - ship.x))) );
        }
}

void Enemy::lvl6(const SDL_Rect ship, int status) {
        if(!flag) {
                if(rect.y > 720 || rect.x < 0 - rect.w) {
                        rect.x = 100 + rng(1280-100);
                        rect.y = -49 * 300;
                        angle = 270;
                        ++passes;
                }

                if(rect.y < 0) rect.y += ENEMY_VEL;
                else this->lvl6_enemy_move(ship, status);
        } else if(flag == 1) {
                if(rect.x < 0 - rect.w) {
                        rect.x = 69 * 300;
                        rect.y = rng(720);
                        angle = 0;
                        ++passes;
                }

                if(rect.x > 1280 - rect.w) rect.x -= ENEMY_VEL;
                else this->lvl6_enemy_move(ship, status);
        } else {
                if(rect.y < 0 || rect.x < 0 - rect.w) {
                        rect.x = 100 + rng(1280-100);
                        rect.y = 500 + 49 * 300;
                        angle = 90;
                        ++passes;
                }

                if(rect.y > 720 - rect.h) rect.y -= ENEMY_VEL;
                else this->lvl6_enemy_move(ship, status);
        }
}

void Enemy::lvl6_enemy_move(const SDL_Rect ship, int status) {
        rect.x -= rint(ENEMY_VEL * cos(DEGRAD(angle)));
        rect.y -= rint(ENEMY_VEL * sin(DEGRAD(angle)));
        if(status) angle = RADDEG( atan(((double) (rect.y - ship.y)) / ((double) abs(rect.x - ship.x))) );
}

void Boss::boss_battle(GameData &data, std::shared_ptr<Mix_Chunk> laser) {
        if(status != ALIVE) return;
        if(rect.x > 2*1280/3) {
                rect.x -= ENEMY_VEL;
                return;
        }

        if(!bullet_timeout) {
                switch(pattern) {
                        case 3:
                        case 1:         if(!change && angles[0] == 60) change = true;
                                        else if(change && angles[2] == 300) change = false;

                                        for(size_t i = 0; i < (pattern == 1 ? 3 : 5); ++i) {
                                                if(!change) angles[i] = (angles[i] + 5) % 360;
                                                else angles[i] = (angles[i] - 5 + 360) % 360;
                                        } break;

                        case 4:
                        case 2:         if(!change && angles[0] == 300) change = true;
                                        else if(change && angles[0] == 60) change = false;

                                        if(!change) {
                                                angles[0] = (angles[0] - 5 + 360) % 360;
                                                angles[2] = (angles[2] + 5) % 360;
                                        } else {
                                                angles[0] = (angles[0] + 5) % 360;
                                                angles[2] = (angles[2] - 5 + 360) % 360;
                                        } break;
                }
                data.bullets.create_bb(rect, angles[0]);
                data.bullets.create_bb(rect, angles[1]);
                data.bullets.create_bb(rect, angles[2]);
                if(pattern >= 3) {
                        data.bullets.create_bb(rect, angles[3]);
                        data.bullets.create_bb(rect, angles[4]);
                }
                if(!data.muted) Mix_PlayChannel(-1, laser.get(), 0);

                bullet_timeout = BOSS_BULLET_TIMEOUT;
        } else --bullet_timeout;

        if(!pattern_timeout) {
                pattern_timeout = (BOSS_BULLET_TIMEOUT+1) * (30 / 5) * 8;
                pattern = pattern % 4 + 1;
        } else --pattern_timeout;
}
