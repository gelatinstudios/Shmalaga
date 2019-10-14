
class WinRend {
public:
        std::shared_ptr<SDL_Window> win;
        std::shared_ptr<SDL_Renderer> rend;
        WinRend(void);
        ~WinRend(void);

};

static inline void render_surf(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<SDL_Surface> surf, const SDL_Rect rect) {
        SDL_Texture *text = SDL_CreateTextureFromSurface(rend.get(), surf.get());
        SDL_RenderCopy(rend.get(), text, NULL, &rect);
        SDL_DestroyTexture(text);
}
