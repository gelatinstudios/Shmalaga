#ifndef BULLETS_H
#define BULLETS_H

#define BULLET_VEL 12

struct BB {
        SDL_Point point;
        double angle;
        double dx, dy;
};

struct Bullets {
        std::vector<SDL_Point> gb;
        std::vector<BB> bb;
        Uint8 bullet_timeout = 0;
        Uint8 boss_bullet_timeout = 0;

        void create_gb(SDL_Rect);
        void create_bb(const SDL_Rect, double);
};

#endif
