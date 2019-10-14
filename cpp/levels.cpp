
#include "essentials.hpp"

#define STRT_POS (1280*2)
#define ENEMY_VEL 8
#define SQRT3 1.732050807568877

void GameData::load_level(void) {
        typedef void (GameData::*level_ptr)();
        level_ptr load_level_ptr[] = {&GameData::load_level_1,
                                            &GameData::load_level_2,
                                            &GameData::load_level_3,
                                            &GameData::load_level_4,
                                            &GameData::load_level_5,
                                            &GameData::load_level_6,
                                            &GameData::load_level_7,};

        (this->*load_level_ptr[level-1])();

        if(level >= 2 && level < 7) {
                gold_enemy.status = ALIVE;
                gold_enemy.rect.x = 1280*3;
                gold_enemy.rect.y = rng(720/3) + 720/3;
        }

        for(auto &enemy : enemies) {
                enemy.status = ALIVE;
                enemy.passes = 0;
        }

        level_timeout = LEVEL_TIMEOUT;
}

void GameData::load_level_1(void) {
        enemies.resize(1);

        enemies[0].rect.x = 1280 * 2;
        enemies[0].rect.y = (729 - 60) / 2;
}

void GameData::load_level_2(void) {
        enemies.resize(201);

        for(size_t j = 0; j < 3; ++j) {
                const size_t n = j * 67;

                //central line
                enemies[n+0].rect.x = STRT_POS*(j+1);
                enemies[n+1].rect.x = STRT_POS*(j+1) + 260;
                enemies[n+2].rect.x = STRT_POS*(j+1) + 665;
                enemies[n+3].rect.x = STRT_POS*(j+1) + 1196;
                enemies[n+4].rect.x = STRT_POS*(j+1) + 1766;
                for(size_t i = 0; i < 5; ++i)
                        enemies[n+i].rect.y = 720/2 - enemies[n+i].rect.h/2;

                //first diag
                enemies[n+5].rect.x = STRT_POS*(j+1) + 100;
                enemies[n+6].rect.x = STRT_POS*(j+1) + 206;
                enemies[n+7].rect.x = STRT_POS*(j+1) + 318;
                enemies[n+8].rect.x = STRT_POS*(j+1) + 414;
                enemies[n+9].rect.x = STRT_POS*(j+1) + 510;
                enemies[n+5].rect.y = 269;
                enemies[n+6].rect.y = 210;
                enemies[n+7].rect.y = 150;
                enemies[n+8].rect.y = 85;
                enemies[n+9].rect.y = 20;

                //second diag
                enemies[n+10].rect.x = STRT_POS*(j+1) + 372;
                enemies[n+11].rect.x = STRT_POS*(j+1) + 480;
                enemies[n+12].rect.x = STRT_POS*(j+1) + 580;
                enemies[n+13].rect.x = STRT_POS*(j+1) + 684;
                enemies[n+14].rect.x = STRT_POS*(j+1) + 788;
                enemies[n+10].rect.y = 263;
                enemies[n+11].rect.y = 215;
                enemies[n+12].rect.y = 162;
                enemies[n+13].rect.y = 110;
                enemies[n+14].rect.y = 52;

                //first curve
                enemies[n+15].rect.x = STRT_POS*(j+1) + 765;
                enemies[n+16].rect.x = STRT_POS*(j+1) + 870;
                enemies[n+17].rect.x = STRT_POS*(j+1) + 975;
                enemies[n+18].rect.x = STRT_POS*(j+1) + 1090;
                enemies[n+19].rect.x = STRT_POS*(j+1) + 1206;
                enemies[n+20].rect.x = STRT_POS*(j+1) + 1323;
                enemies[n+21].rect.x = STRT_POS*(j+1) + 1442;
                enemies[n+22].rect.x = STRT_POS*(j+1) + 1545;
                enemies[n+23].rect.x = STRT_POS*(j+1) + 1652;
                enemies[n+24].rect.x = STRT_POS*(j+1) + 1762;
                enemies[n+25].rect.x = STRT_POS*(j+1) + 1864;
                enemies[n+15].rect.y = 262;
                enemies[n+16].rect.y = 218;
                enemies[n+17].rect.y = 180;
                enemies[n+18].rect.y = 150;
                enemies[n+19].rect.y = 124;
                enemies[n+20].rect.y = 120;
                enemies[n+21].rect.y = 114;
                enemies[n+22].rect.y = 104;
                enemies[n+23].rect.y = 84;
                enemies[n+24].rect.y = 58;
                enemies[n+25].rect.y = 26;

                //second curve
                enemies[n+26].rect.x = STRT_POS*(j+1) + 1320;
                enemies[n+27].rect.x = STRT_POS*(j+1) + 1450;
                enemies[n+28].rect.x = STRT_POS*(j+1) + 1582;
                enemies[n+29].rect.x = STRT_POS*(j+1) + 1710;
                enemies[n+30].rect.x = STRT_POS*(j+1) + 1834;
                enemies[n+31].rect.x = STRT_POS*(j+1) + 1946;
                enemies[n+32].rect.x = STRT_POS*(j+1) + 2052;
                enemies[n+33].rect.x = STRT_POS*(j+1) + 2164;
                enemies[n+26].rect.y = 250;
                enemies[n+27].rect.y = 222;
                enemies[n+28].rect.y = 198;
                enemies[n+29].rect.y = 184;
                enemies[n+30].rect.y = 162;
                enemies[n+31].rect.y = 126;
                enemies[n+32].rect.y = 74;
                enemies[n+33].rect.y = 40;

                //last diag
                enemies[n+34].rect.x = STRT_POS*(j+1) + 1984;
                enemies[n+35].rect.x = STRT_POS*(j+1) + 2188;
                enemies[n+34].rect.y = 270;
                enemies[n+35].rect.y = 224;

                //mirror
                for(size_t i = 0; i < 31; ++i) {
                        enemies[n+i+36].rect.x = enemies[n+i+5].rect.x;
                        enemies[n+i+36].rect.y = 720-60 - enemies[n+i+5].rect.y;
                }
        }
}

void GameData::load_level_3(void) {
        enemies.resize(150);

        for(size_t i = 0; i < 25; ++i) {
                for(size_t j = 0; j < 3; ++j) {
                        enemies[j + i*3].rect.x = 350 + j*326 + rng(163) - 81;
                        enemies[j + i*3].rect.y = -i*200 - 200 + rng(100) - 50;
                        enemies[j + i*3].angle = 300;
                        enemies[j + i*3].flag = 0;
                }
        }

        for(size_t i = 0; i < 75; ++i) {
                enemies[i+75].rect.x = enemies[i].rect.x + 50;
                enemies[i+75].rect.y = 1280*2-60 - enemies[i].rect.y;
                enemies[i+75].angle = 60;
                enemies[i+75].flag = 1;
        }
}
void GameData::load_level_4(void) {
        enemies.resize(150);

        for(size_t i = 0; i < 25; ++i) {
                for(size_t j = 0; j < 3; ++j) {
                        enemies[j + i*3].rect.x = 200 + j*326 + rng(163) - 81;
                        enemies[j + i*3].rect.y = -i*400 - 200 + rng(200) - 100;
                        enemies[j + i*3].angle = 240;
                        enemies[j + i*3].flag = 0;
                }
        }

        for(size_t i = 0; i < 75; ++i) {
                enemies[i+75].rect.x = enemies[i].rect.x + 50;
                enemies[i+75].rect.y = 1280*2-60 - enemies[i].rect.y;
                enemies[i+75].angle = 120;
                enemies[i+75].flag = 1;
        }
}

void GameData::load_level_5(void) {
        enemies.resize(70);

        for(size_t i = 0; i < 7; ++i) {
                for(size_t j = 0; j < 10; ++j) {
                        enemies[j + i*10].rect.x = STRT_POS + 1280*i;
                        enemies[j + i*10].rect.y = 72*j;
                        enemies[j + i*10].angle = 0;
                }
        }
}

void GameData::load_level_6(void) {
        enemies.resize(150);

        for(size_t j = 0; j < 50; ++j) {
                enemies[j].rect.x = 100 + rng(1280-100);
                enemies[j].rect.y = -500 - j * 300;
                enemies[j].angle = 270;
                enemies[j].flag = 0;

                for(size_t k = 0; k < j; ++k)
                        if(collision_detect(enemies[j].rect, enemies[k].rect))
                                --j;
        }
        for(size_t j = 0; j < 50; ++j) {
                enemies[j+50].rect.x = STRT_POS + j * 300;
                enemies[j+50].rect.y = rng(720);
                enemies[j+50].angle = 0;
                enemies[j].flag = 1;

                for(size_t k = 0; k < j; ++k)
                        if(collision_detect(enemies[j+50].rect, enemies[k+50].rect))
                                --j;
        }
        for(size_t j = 0; j < 50; ++j) {
                enemies[j+2*50].rect.x = 100 + rng(1280-100);
                enemies[j+2*50].rect.y = 720 + 500 + j * 300;
                enemies[j+2*50].angle = 90;
                enemies[j].flag = 2;

                for(size_t k = 0; k < j; ++k)
                        if(collision_detect(enemies[j+2*50].rect, enemies[k+2*50].rect))
                                --j;
        }
}

void GameData::load_level_7(void) {
        enemies.clear();

        gold_enemy.status = DEAD;

        boss.status = ALIVE;

        boss.rect.x = 1280 * 2;
        boss.rect.y = (720 - boss.rect.h) / 2;
}
