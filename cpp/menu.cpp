
#include "essentials.hpp"

#define VOL_SCALE 8

static inline void render_as_selected(std::shared_ptr<SDL_Renderer>, std::shared_ptr<SDL_Texture>, const SDL_Rect &);
static inline void render_key(std::shared_ptr<SDL_Renderer>, std::shared_ptr<TTF_Font>, const SDL_Rect &, const char *, const char *, bool, Uint8);
static inline void render_volume(std::shared_ptr<SDL_Renderer>, std::shared_ptr<TTF_Font>, const SDL_Rect &, const char *, bool, int);

bool menu_handler(GameData &data, SDL_Event *event) {
        bool quit = false;

        if(event->type != SDL_KEYDOWN) return false;
        switch(data.gamestate) {
                case MENU:
                        switch(event->key.keysym.sym) {
                                case SDLK_ESCAPE:       data.selected = 0;
                                                        data.gamestate = IN_GAME;
                                                        break;

                                case SDLK_UP:           data.selected = (data.selected - 1 + 3) % 3;
                                                        break;

                                case SDLK_DOWN:         data.selected = (data.selected + 1) % 3;
                                                        break;

                                case SDLK_RETURN:       switch(data.selected) {
                                                                case 0: data.gamestate = CONTROLS;
                                                                        data.selected = 0;
                                                                        break;

                                                                case 1: data.gamestate = SOUND;
                                                                        data.selected = 0;
                                                                        break;

                                                                case 2: quit = true;
                                                        } break;
                        } break;
                case CONTROLS:
                        switch(event->key.keysym.sym) {
                                case SDLK_UP:           if(data.selected == 10) data.selected = 4;
                                                        else if(data.selected == 5) data.selected = 10;
                                                        else  data.selected = (data.selected - 1 + 11) % 11;
                                                        break;

                                case SDLK_DOWN:         if(data.selected == 4) data.selected = 10;
                                                        else data.selected = (data.selected + 1) % 11;
                                                        break;

                                case SDLK_LEFT:         if(data.selected > 4 && data.selected != 10) data.selected -= 5;
                                                        break;

                                case SDLK_RIGHT:        if(data.selected < 5) data.selected += 5;
                                                        break;

                                case SDLK_ESCAPE:       data.selected = 0;
                                                        data.gamestate = IN_GAME;
                                                        break;

                                case SDLK_RETURN:       if(data.selected == 10) {
                                                                data.selected = 0;
                                                                data.gamestate = MENU;
                                                        } else data.gamestate = KEYSET_MODE;
                                                        return 0;
                        } break;
                case KEYSET_MODE:
                        if(data.selected < 5) data.keys[data.selected] = event->key.keysym.scancode;
                        else data.keys[data.selected] = event->key.keysym.sym;
                        data.gamestate = CONTROLS;
                        return 0;
                case SOUND:
                        switch(event->key.keysym.sym) {
                                case SDLK_RETURN:       if(data.selected == 3) data.muted = !data.muted;
                                                        else if(data.selected == 4) {
                                                                data.selected = 0;
                                                                data.gamestate = MENU;
                                                        } break;

                                case SDLK_UP:           data.selected = (data.selected - 1 + 5) % 5;
                                                        break;

                                case SDLK_DOWN:         data.selected = (data.selected + 1) % 5;
                                                        break;

                                case SDLK_ESCAPE:       data.selected = 0;
                                                        data.gamestate = IN_GAME;
                                                        break;
                        }

                        const Uint8 *state = SDL_GetKeyboardState(NULL);
                        if(state[SDL_SCANCODE_LEFT]) {
                                switch(data.selected) {
                                        case 0: if(data.volumes.master > 0) data.volumes.master -= VOL_SCALE; break;
                                        case 1: if(data.volumes.music > 0) data.volumes.music -= VOL_SCALE; break;
                                        case 2: if(data.volumes.sfx > 0) data.volumes.sfx -= VOL_SCALE; break;
                                }

                                Mix_VolumeMusic(data.volumes.music * data.volumes.master / MIX_MAX_VOLUME);
                                Mix_Volume(-1, data.volumes.sfx * data.volumes.master / MIX_MAX_VOLUME);
                        }
                        if(state[SDL_SCANCODE_RIGHT]) {
                                switch(data.selected) {
                                        case 0: if(data.volumes.master < MIX_MAX_VOLUME) data.volumes.master += VOL_SCALE; break;
                                        case 1: if(data.volumes.music < MIX_MAX_VOLUME) data.volumes.music += VOL_SCALE; break;
                                        case 2: if(data.volumes.sfx < MIX_MAX_VOLUME) data.volumes.sfx += VOL_SCALE; break;
                                }

                                Mix_VolumeMusic(data.volumes.music * data.volumes.master / MIX_MAX_VOLUME);
                                Mix_Volume(-1, data.volumes.sfx * data.volumes.master / MIX_MAX_VOLUME);
                        }
                        break;
        }

        return quit;
}

void render_menu(GameData &data, std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, std::shared_ptr<SDL_Texture> menu[]) {
        SDL_SetRenderDrawColor(rend.get(), 255, 255, 255, 255);

        const SDL_Rect title = {1280/2 - 250, 80, 500, 75};
        switch(data.gamestate) {
                case MENU: {
                        const SDL_Rect controls = {1280/2 - 175, 75+80+40, 350, 50};
                        const SDL_Rect sound = {1280/2 - 100, 75+80+40+50+40, 200, 50};
                        const SDL_Rect quit = {1280/2 - 100, 75+80+3*40+2*50, 200, 50};

                        SDL_RenderCopy(rend.get(), menu[MN_TITLE].get(), NULL, &title);

                        if(data.selected == 0) render_as_selected(rend, menu[MN_CONTROLS], controls);
                        else SDL_RenderCopy(rend.get(), menu[MN_CONTROLS].get(), NULL, &controls);

                        if(data.selected == 1) render_as_selected(rend, menu[MN_SOUND], sound);
                        else SDL_RenderCopy(rend.get(), menu[MN_SOUND].get(), NULL, &sound);

                        if(data.selected == 2) render_as_selected(rend, menu[MN_QUIT], quit);
                        else SDL_RenderCopy(rend.get(), menu[MN_QUIT].get(), NULL, &quit);
                        break;
                }
                case CONTROLS:
                case KEYSET_MODE: {
                        const char *actions[] = {"MOVE UP", "MOVE DOWN", "MOVE LEFT", "MOVE RIGHT", "SHOOT", "MENU", "PAUSE", "FULLSCREEN", "MUTE", "RESET"};

                        SDL_Rect rect = {100, 80, 490, 75};
                        for(size_t i = 0; i < 10; ++i) {
                                if(i == 5) {
                                        rect.x = 200+490;
                                        rect.y = 80;
                                }
                                rect.y += 90;

                                const char *keyname = i < 5 ? SDL_GetScancodeName((SDL_Scancode) data.keys[i]) : SDL_GetKeyName(data.keys[i]);
                                render_key(rend, font, rect, actions[i], keyname, data.selected == i, data.gamestate);
                        }

                        const SDL_Rect back = {1280/2 - 100, 80+6*75+6*15, 200, 50};

                        SDL_RenderCopy(rend.get(), menu[MN_CONTROLS].get(), NULL, &title);

                        if(data.selected == 10) render_as_selected(rend, menu[MN_BACK], back);
                        else SDL_RenderCopy(rend.get(), menu[MN_BACK].get(), NULL, &back);
                        break;
                }
                case SOUND: {
                        const SDL_Rect master = {1280/2 - 400, 75+80+40, 800, 50};
                        const SDL_Rect music = {1280/2 - 400, 75+80+40+50+40, 800, 50};
                        const SDL_Rect sfx = {1280/2 - 400, 75+80+3*40+2*50, 800, 50};
                        const SDL_Rect mute = {1280/2 - 200, 75+80+4*40+3*50, 400, 50};
                        const SDL_Rect back = {1280/2 - 100, 75+80+5*40+4*50, 200, 50};

                        SDL_RenderCopy(rend.get(), menu[MN_SOUND].get(), NULL, &title);

                        render_volume(rend, font, master, "MASTER VOLUME", data.selected == 0, data.volumes.master);
                        render_volume(rend, font, music, "MUSIC VOLUME", data.selected == 1, data.volumes.music);
                        render_volume(rend, font, sfx, "SFX VOLUME", data.selected == 2, data.volumes.sfx);

                        char mute_str[16] = "MUTE: ";
                        strcat(mute_str, data.muted ? "SOUND OFF" : "SOUND ON");
                        SDL_Surface *mute_surf;
                        if(data.selected == 3) mute_surf = TTF_RenderText_Blended(font.get(), mute_str, gold);
                        else mute_surf = TTF_RenderText_Blended(font.get(), mute_str, white);
                        render_surf(rend, std::shared_ptr<SDL_Surface>(mute_surf, SDL_FreeSurface), mute);

                        if(data.selected == 4) render_as_selected(rend, menu[MN_BACK], back);
                        else SDL_RenderCopy(rend.get(), menu[MN_BACK].get(), NULL, &back);
                        break;
                }
        }

        SDL_RenderPresent(rend.get());
        SDL_SetRenderDrawColor(rend.get(), 0, 0, 0, 0);
}

static inline void render_as_selected(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<SDL_Texture> text, const SDL_Rect &rect) {
        SDL_SetTextureColorMod(text.get(), 255, 215, 0);
        SDL_RenderCopy(rend.get(), text.get(), NULL, &rect);
        SDL_SetTextureColorMod(text.get(), 255, 255, 255);
}

static inline void render_key(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, const SDL_Rect &rect, const char *action, const char *keyname, bool selected, Uint8 mode) {
        char str[50] = {0};
        sprintf(str, "%s: %s", action, keyname);

        SDL_Surface *surf;
        if(mode == KEYSET_MODE && selected) surf = TTF_RenderText_Blended(font.get(), "PRESS A KEY", gold);
        else if(selected) surf = TTF_RenderText_Blended(font.get(), str, gold);
        else surf = TTF_RenderText_Blended(font.get(), str, white);
        render_surf(rend, std::shared_ptr<SDL_Surface>(surf, SDL_FreeSurface), rect);
}

static inline void render_volume(std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, const SDL_Rect &rect, const char *barname, bool selected, int volume) {
        char bar[] = "-----------";
        bar[10*volume/MIX_MAX_VOLUME] = '|';
        char str[27] = {0};
        sprintf(str, "%s: %s", barname, bar);

        SDL_Surface *surf;
        if(selected) surf = TTF_RenderText_Blended(font.get(), str, gold);
        else surf = TTF_RenderText_Blended(font.get(), str, white);
        render_surf(rend, std::shared_ptr<SDL_Surface>(surf, SDL_FreeSurface), rect);
}
