
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#define DEGRAD(x) ((x) * (M_PI/180.0))
#define RADDEG(x) ((x) * (180.0/M_PI))

#define LNGTH(x) ((sizeof (x)) / (sizeof (x[0])))

#define GB_W 15
#define GB_H 10
#define BB_W 12
#define BB_H 8

#define MK_GB_RECT(a) (const SDL_Rect){a.x, a.y, GB_W, GB_H}
#define MK_BB_RECT(a) (const SDL_Rect){a.x, a.y, BB_W, BB_H}

static size_t rng_state = 0;

static inline Uint32 pcg_rand() {
        rng_state = rng_state * 5 + 1;
        return rng_state >> (29 - (rng_state >> 61));
}

static inline int rng(int n) {
        int x;
        do { x = pcg_rand();} while(x >= (RAND_MAX - RAND_MAX % n));
        return x % n;

}

static inline bool collision_detect(SDL_Rect rect1, SDL_Rect rect2) {
        const bool x = (rect1.x < rect2.x + rect2.w) && (rect1.x + rect1.w > rect2.x);
        const bool y = (rect1.y < rect2.y + rect2.h) && (rect1.y + rect1.h > rect2.y);
        return x && y;
}

static inline bool is_on_screen(SDL_Rect rect) {
        const bool x = rect.x + rect.w > 0 && rect.x < 1280;
        const bool y = rect.y + rect.h > 0 && rect.y < 720;
        return x && y;
}

#endif
