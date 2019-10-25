
#include "essentials.hpp"

static inline void render_player(GameData &, std::shared_ptr<SDL_Renderer>, Textures &);
static inline void render_scores(GameData &, std::shared_ptr<SDL_Renderer>, std::shared_ptr<TTF_Font>);

const SDL_Rect muted_rect = {10, 720-30-10, 250, 30};
const SDL_Rect lives_rect = {1280-100-10, 720-20-50, 100, 20};
const SDL_Rect dead_rect = {(1280 - 600) / 2, 390-100+10, 600, 100};
const SDL_Rect pressr_rect = {(1280 - 310) / 2, 400, 310, 50};
const SDL_Rect pause_rect = {640-250+10, 390-(390/2)-100, 500, 100};
const SDL_Rect press_enter_rect = {(1280 - 900) / 2, (720 - 100) / 2, 900, 100};
const SDL_Rect one_up_rect = {10, 10+25+10, 50, 20};
const SDL_Rect stage_rect = {(1280 - 400) / 2, (720 - 55) / 2, 400, 55};

void render(GameData &data, std::shared_ptr<SDL_Renderer> rend, Assets &assets) {
        //music                               VVVthis is for the menuVVV
        if(data.gamestate == IN_GAME || (data.gamestate == SOUND && data.selected <= 1)) {
                if(!data.muted && Mix_PlayingMusic() && Mix_PausedMusic()) Mix_ResumeMusic();
                if(data.muted && !Mix_PausedMusic()) Mix_PauseMusic();
        } else Mix_PauseMusic();

        if(!(data.secret & 0x01)) SDL_RenderClear(rend.get());

        //stars
        for(auto &star : data.stars) {
                if(!is_on_screen(star.rect)) continue;
                SDL_SetRenderDrawColor(rend.get(), star.color.r, star.color.g, star.color.b, 255);
                SDL_RenderFillRect(rend.get(), &star.rect);
        }

        SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 0);

        if(data.gamestate == STARTING_SCREEN) {
                SDL_RenderCopy(rend.get(), assets.textures.texts[TXT_START_SCREEN].get(), NULL, &press_enter_rect);
                SDL_RenderPresent(rend.get());
                return;
        } else if(data.gamestate == NAME_ENTRY) {
                render_name(data, rend, assets.textures.texts, assets.font);
                return;
        } else if(data.gamestate == LEADERBOARD) {
                data.leaderboard.render(rend, assets.textures.texts, assets.font);
                return;
        }

        render_player(data, rend, assets.textures);

        for(auto &enemy : data.enemies)
                enemy.render(data.sparkle, assets.font, rend, assets.textures);
        data.gold_enemy.render(data.sparkle, assets.font, rend, assets.textures);

        data.boss.render(rend, assets.textures.sprites);

        //bullets
        for(auto gb : data.bullets.gb) {
                const SDL_Rect rect = MK_GB_RECT(gb);
                SDL_RenderCopy(rend.get(), assets.textures.sprites[SPR_GB].get(), NULL, &rect);
        }
        for(auto bb : data.bullets.bb) {
                const SDL_Rect rect = MK_BB_RECT(bb.point);
                SDL_RenderCopyEx(rend.get(), assets.textures.sprites[SPR_BB].get(), NULL, &rect, bb.angle, NULL, SDL_FLIP_NONE);
        }

        render_scores(data, rend, assets.font);

        //"STAGE N"
        if(data.gamestate == IN_GAME && data.level_timeout) {
                if(data.secret & 0x01) SDL_RenderClear(rend.get());
                char stage[10] = {0};
                sprintf(stage, "STAGE %u", data.level);
                std::shared_ptr<SDL_Surface> surf = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(assets.font.get(), stage, white), SDL_FreeSurface);
                render_surf(rend, surf, stage_rect);
                --data.level_timeout;
        }

        //"sound muted"
        if(data.muted) SDL_RenderCopy(rend.get(), assets.textures.texts[TXT_MUTED].get(), NULL, &muted_rect);

        //lives
        SDL_RenderCopy(rend.get(), assets.textures.texts[TXT_LIVES].get(), NULL, &lives_rect);
        for(int i = 0; i < data.lives - 1; ++i) {
                const SDL_Rect life = {1280-30*(i+1)-10, 720-30-10, 30, 30};
                SDL_RenderCopy(rend.get(), assets.textures.sprites[SPR_SHIP].get(), NULL, &life);
        }

        //"YOU'RE DEAD"
        if(data.player_status == DEAD && !data.lives && data.gamestate < MENU) {
                SDL_RenderCopy(rend.get(), assets.textures.texts[TXT_YOU_DIED].get(), NULL, &dead_rect);
                SDL_RenderCopy(rend.get(), assets.textures.texts[TXT_PRESSR].get(), NULL, &pressr_rect);
        }

        switch(data.gamestate) {
                case IN_GAME:
                                SDL_RenderPresent(rend.get());
                                break;
                case PAUSED:
                                if(!Mix_PausedMusic()) Mix_PauseMusic();
                                SDL_RenderCopy(rend.get(), assets.textures.texts[TXT_PAUSED].get(), NULL, &pause_rect);
                                SDL_RenderPresent(rend.get());
                                break;
                case MENU:
                case CONTROLS:
                case KEYSET_MODE:
                case SOUND:
                                render_menu(data, rend, assets.font, assets.textures.menu);
                                break;
                default:
                                std::cout << "gamestate error. fatal.\nterminating.\n";
                                exit(EXIT_FAILURE);
                                break;
        }
}

static inline void render_player(GameData &data, std::shared_ptr<SDL_Renderer> rend, Textures &texts) {
                if(data.player_status == ALIVE)
                        SDL_RenderCopy(rend.get(), texts.sprites[SPR_SHIP].get(), NULL, &data.ship);
                else if(data.player_status != DEAD)
                        SDL_RenderCopy(rend.get(), texts.sprites[SPR_EXPLOSION].get(), &explosion_clips[data.player_status-1], &data.ship);
                else if(data.player_status == DEAD && data.lives && !rng(4))
                        SDL_RenderCopy(rend.get(), texts.sprites[SPR_SHIP].get(), NULL, &data.ship);
}

void Enemy::render(Sparkle &sparkle, std::shared_ptr<TTF_Font> font, std::shared_ptr<SDL_Renderer> rend, Textures &texts) {
        if(is_on_screen(rect)) {
                if(status == ALIVE) {
                        SDL_RenderCopyEx(rend.get(), texts.sprites[texture].get(), NULL, &rect, angle, NULL, SDL_FLIP_NONE);

                        if(texture == SPR_GOLD_ENEMY) {
                                if(!sparkle.timeout) {
                                        sparkle.rect.x = rng(rect.w) + rect.x;
                                        sparkle.rect.y = rng(rect.h) + rect.y;
                                        sparkle.rect.w = sparkle.rect.h = (rng(2) ? 0 : 10);
                                        sparkle.timeout = SPARKLE_TIMEOUT;
                                } else --sparkle.timeout;
                                SDL_RenderCopy(rend.get(), texts.sparkle.get(), NULL, &sparkle.rect);
                        }
                } else if(status != DEAD) {
                        SDL_RenderCopy(rend.get(), texts.sprites[SPR_EXPLOSION].get(), &explosion_clips[status-1], &rect);

                        char points[5] = {0};
                        if(texture == SPR_GOLD_ENEMY) sprintf(points, "%u", 2*ENEM_P);
                        else  sprintf(points, "%u", ENEM_P / (unsigned) pow(2, passes));
                        std::shared_ptr<SDL_Surface> surf = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(font.get(), points, gold), SDL_FreeSurface);
                        const int width = 10 * strlen(points);
                        const SDL_Rect points_rect = {rect.x + (rect.w - width) / 2, rect.y - 12, width, 10};
                        render_surf(rend, surf, points_rect);
                }
        }
}

void Boss::render(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<SDL_Texture> sprites[]) {
        if(status == ALIVE) {
                if(damage_timeout) {
                        SDL_SetTextureColorMod(sprites[SPR_BOSS].get(), 255, 0, 0);
                        SDL_RenderCopy(rend.get(), sprites[SPR_BOSS].get(), &boss_clips[3 - (4 * (hp-1) / BOSS_HP)], &rect);
                        SDL_SetTextureColorMod(sprites[SPR_BOSS].get(), 255, 255, 255);
                        --damage_timeout;
                }
                else SDL_RenderCopy(rend.get(), sprites[SPR_BOSS].get(), &boss_clips[3 - (4 * (hp-1) / BOSS_HP)], &rect);
        } else if(status != DEAD) {
                if(new_exp_timeout || explosions[0].timeout) {
                        SDL_RenderCopy(rend.get(), sprites[SPR_BOSS].get(), &boss_clips[3], &rect);
                        for(size_t i = 0; i < NUM_EXP; ++i) {
                                if(explosions[i].timeout)
                                        SDL_RenderCopy(rend.get(), sprites[SPR_EXPLOSION].get(), &explosion_clips[3 - (explosions[i].timeout / EXPLOSION_TIMEOUT)], &explosions[i].rect);
                        }
                } else {
                        SDL_RenderCopy(rend.get(), sprites[SPR_BOSS].get(), &boss_clips[3], &rect);
                        SDL_RenderCopy(rend.get(), sprites[SPR_EXPLOSION].get(), &explosion_clips[status-1], &rect);
                }
        }
}

static inline void render_scores(GameData &data, std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font) {
        char score[50] = {0};
        char highscore[50] = {0};
        sprintf(score, "SCORE: %u", data.score.val);
        sprintf(highscore, "HIGHSCORE: %u", data.leaderboard.scores[10].val);

        std::shared_ptr<SDL_Surface> score_surf = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(font.get(), score, white), SDL_FreeSurface);
        std::shared_ptr<SDL_Surface> highscore_surf = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(font.get(), highscore, white), SDL_FreeSurface);

        const SDL_Rect score_rect = {10, 10, 200, 25};
        const SDL_Rect highscore_rect = {1000, 10, 270, 25};

        render_surf(rend, score_surf, score_rect);
        render_surf(rend, highscore_surf, highscore_rect);

        if(data.one_up_timeout) {
                std::shared_ptr<SDL_Surface> one_up_surf = std::shared_ptr<SDL_Surface>(TTF_RenderText_Blended(font.get(), "1UP", gold), SDL_FreeSurface);
                render_surf(rend, one_up_surf, one_up_rect);
                --data.one_up_timeout;
        }
}
