#ifndef DATA_H
#define DATA_H

//array macros
#define MAX_ENEMIES 500
#define MAX_BULLETS 30
//#define MAX_ENEMY_BULLETS 20
#define MAX_ENEMY_BULLETS 10000
#define MAX_STARS 1250

#define ENEM_P 500

#define LEVEL_TIMEOUT 45
#define ONE_UP_TIMEOUT 15
#define RESPAWN_TIMEOUT 120
#define EXPLOSION_TIMEOUT 5 //how many frames each explosion sprite stays up for
#define BULLET_TIMEOUT 10 //how many frames pass before you can shoot again
#define MAX_STAR_POS 5000 //length of the star cycle

//player/enemy status
enum {ALIVE, DYING_1, DYING_2, DYING_3, DYING_4, DEAD};

//gamestates
enum {IN_GAME, PAUSED, STARTING_SCREEN, NAME_ENTRY, LEADERBOARD, MENU, CONTROLS, KEYSET_MODE, SOUND};

//highscore key
enum {KEY3, KEY1, KEY4, KEY2};
#define KEY 8
#define ENC_KEY 48

//structures
typedef struct {
        Uint8 r, g, b;
} Color;

typedef struct {
        SDL_Rect rect;
        Color color;
} Star;

typedef struct {
        SDL_Rect explosion[4];
        SDL_Rect gold_sprites[4];
} Clips;

typedef struct {
        SDL_Rect rect;
        Uint8 status;
        Uint8 explosion_timeout;

        double angle;
        Uint8 circle;
        Uint8 passes;

        Uint8 texture;
} Enemy;

typedef struct {
        SDL_Rect rect;
        double angle;
        double dx, dy;
} BB;

typedef struct {
        SDL_Rect rect;
        Uint8 status;
        Uint8 explosion_timeout;
        Uint8 hp;
} Boss;

enum {UP_K, DOWN_K, LEFT_K, RIGHT_K, SHOOT_K, MENU_K, PAUSE_K, FSCREEN_K, MUTE_K, RESET_K};
typedef struct {
        SDL_Keycode keys[10];

        Uint8 selected;

        Uint8 fullscreen;
        Uint8 muted;

        const SDL_Keycode *sekritptr;
        Uint8 secret;

        Uint8 gamestate;
        Uint8 level;
        Uint8 level_timeout;

        Score leaderboard[10];
        Score score;

        Uint8 lives;
        Uint8 respawn_timeout;
        Uint8 life_milestone;
        Uint8 one_up_timeout;

        Star *stars;

        Clips clips;

        SDL_Rect ship;

        Uint8 player_status;
        Uint8 player_death_timeout;

        SDL_Rect gb[MAX_BULLETS];
        size_t gb_count;
        Uint8 bullet_timeout;

        Enemy *enemies;
        unsigned enemy_count;
        unsigned living_enemies;

        Enemy gold_enemy;
        Boss boss;

        BB bb[MAX_ENEMY_BULLETS];
        size_t bb_count;
} GameData;

extern const SDL_Keycode lsd_code[10];
extern const SDL_Keycode lives_code[10];
extern const SDL_Keycode tiny_code[10];
extern const SDL_Keycode big_code[10];

extern const SDL_Color white;
extern const SDL_Color black;
extern const SDL_Color gold;

int init_data(GameData *);

#endif
