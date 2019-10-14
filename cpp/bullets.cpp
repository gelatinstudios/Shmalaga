
#include "essentials.hpp"

void Bullets::create_gb(SDL_Rect ship) {
        gb.push_back({ship.x + ship.w - 11, ship.y + (ship.h - 10) / 2});
}

void Bullets::create_bb(const SDL_Rect rect, double angle) {
        int x = rint(rect.x + rect.w/2.0 * (1.0 - cos(DEGRAD(angle))) - 12.0/2.0);
        int y = rint(rect.y + rect.h/2.0 * (1.0 - sin(DEGRAD(angle))) - 8.0/2.0);
        double dx = BULLET_VEL * cos(DEGRAD(angle));
        double dy = BULLET_VEL * sin(DEGRAD(angle));

        bb.push_back({{x, y}, angle, dx, dy});
}
