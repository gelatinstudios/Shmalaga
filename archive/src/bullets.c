#include "essentials.h"

void create_gb(GameData *data, Sounds *sounds) {
        data->gb[data->gb_count].x = data->ship.x + data->ship.w - 11;
        data->gb[data->gb_count].y = data->ship.y + data->ship.h/2 - 10/2;
        ++data->gb_count;
        if(!data->muted) Mix_PlayChannel(-1, sounds->laser, 0);
        data->bullet_timeout = BULLET_TIMEOUT;
}

void destroy_gb(GameData *data, int n) {
        for(size_t i = n; i < data->gb_count - 1; ++i)
                data->gb[i] = data->gb[i+1];
        --data->gb_count;
}

void create_bb(GameData *data, Enemy *enemy, Sounds *sounds) {
        data->bb[data->bb_count].rect.x = rint(enemy->rect.x + enemy->rect.w/2.0 * (1.0 - 0.9*cos(DEGRAD(enemy->angle))) - 12.0/2.0);
        data->bb[data->bb_count].rect.y = rint(enemy->rect.y + enemy->rect.h/2.0 * (1.0 - 0.9*sin(DEGRAD(enemy->angle))) - 8.0/2.0);

        data->bb[data->bb_count].dx = BULLET_VEL * cos(DEGRAD(enemy->angle));
        data->bb[data->bb_count].dy = BULLET_VEL * sin(DEGRAD(enemy->angle));
        data->bb[data->bb_count].angle = enemy->angle;

        ++data->bb_count;
        if(!data->muted) Mix_PlayChannel(-1, sounds->bad_laser, 0);
}

void destroy_bb(GameData *data, int n) {
        for(size_t i = n; i < data->bb_count - 1; ++i)
                data->bb[i] = data->bb[i+1];
        --data->bb_count;
}
