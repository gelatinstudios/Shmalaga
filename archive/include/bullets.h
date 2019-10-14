#ifndef BULLETS_H
#define BULLETS_H

#define BULLET_VEL 12

void create_gb(GameData *, Sounds *);
void destroy_gb(GameData *, int);

void create_bb(GameData *, Enemy *, Sounds *);
void destroy_bb(GameData *, int);

#endif
