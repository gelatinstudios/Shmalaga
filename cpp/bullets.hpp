
#define BULLET_VEL 12

typedef struct {
        SDL_Point point;
        double angle;
        double dx, dy;
} BB;

class Bullets {
public:
        std::vector<SDL_Point> gb;
        std::vector<BB> bb;
        Uint8 bullet_timeout = 0;
        Uint8 boss_bullet_timeout = 0;

        void create_gb(SDL_Rect);
        void create_bb(const SDL_Rect, double);
};
