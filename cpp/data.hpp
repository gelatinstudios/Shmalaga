
#define ENEM_P 500

#define LEVEL_TIMEOUT 45
#define ONE_UP_TIMEOUT 60
#define RESPAWN_TIMEOUT 120
#define SPARKLE_TIMEOUT 5
#define EXPLOSION_TIMEOUT 5
#define NEW_EXP_SCALE 9
#define NEW_EXP_TIMEOUT (NEW_EXP_SCALE*NUM_EXP)
#define BOSS_EXPLOSION_TIMEOUT 6
#define BOSS_BULLET_TIMEOUT 12
#define BULLET_TIMEOUT 10
#define DAMAGE_TIMEOUT 6

#define MAX_STARS 1250
#define MAX_STAR_POS 5000

#define BOSS_HP 500

//player/enemy status
enum {ALIVE, DYING_1, DYING_2, DYING_3, DYING_4, DEAD};

//gamestates
enum {IN_GAME, PAUSED, STARTING_SCREEN, NAME_ENTRY, LEADERBOARD, MENU, CONTROLS, KEYSET_MODE, SOUND};

//structures
typedef struct {
        Uint8 r, g, b;
} Color;

typedef struct {
        int master, music, sfx;
} Volumes;

typedef struct {
        SDL_Rect rect;
        Color color;
} Star;

enum {UP_K, DOWN_K, LEFT_K, RIGHT_K, SHOOT_K, MENU_K, PAUSE_K, FSCREEN_K, MUTE_K, RESET_K};
class GameData {
private:
        void load_level_1(void);
        void load_level_2(void);
        void load_level_3(void);
        void load_level_4(void);
        void load_level_5(void);
        void load_level_6(void);
        void load_level_7(void);
public:
        SDL_Keycode keys[10];

        Uint8 selected;

        bool fullscreen;
        bool muted;

        Volumes volumes;

        const SDL_Keycode *sekritptr;
        Uint8 secret;

        Uint8 gamestate;
        Uint8 level;
        Uint8 level_timeout;

        Leaderboard leaderboard;
        Score score;

        Uint8 lives;
        Uint8 respawn_timeout;
        Uint8 life_milestone;
        Uint8 one_up_timeout;

        Star stars[MAX_STARS];

        SDL_Rect ship;

        Uint8 player_status;
        Uint8 player_death_timeout;

        Bullets bullets;

        std::vector<Enemy> enemies;

        Sparkle sparkle;
        Enemy gold_enemy;
        Boss boss;
        GameData(void);
        void reset(void);
        void load_level(void);
};

// extern const SDL_Keycode lsd_code[10];
// extern const SDL_Keycode big_code[10];
// extern const SDL_Keycode tiny_code[10];
// extern const SDL_Keycode lives_code[10];
extern const SDL_Keycode *secret_codes[4];

extern const SDL_Color white;
extern const SDL_Color black;
extern const SDL_Color gold;


extern const SDL_Rect explosion_clips[4];
extern const SDL_Rect boss_clips[4];
