class GameData;

typedef struct {
        SDL_Rect rect;
        Uint8 timeout;
} Sparkle;

class Enemy {
public:
        SDL_Rect rect;
        Uint8 status;
        Uint8 explosion_timeout;

        double angle;
        bool circle;
        Uint8 passes;
        Uint8 flag;

        Uint8 texture;
        Enemy(void);
        bool handler(GameData &, std::shared_ptr<Mix_Chunk>);
        void render(Sparkle &, std::shared_ptr<TTF_Font>, std::shared_ptr<SDL_Renderer>, Textures &);
        void lvl3(void);
        void lvl4(void);
        void mov_circ(void);
        void mov_circ_ccwise(void);
        void lvl5(const SDL_Rect);
        void lvl6(const SDL_Rect, int);
        void lvl6_enemy_move(const SDL_Rect, int);
};
