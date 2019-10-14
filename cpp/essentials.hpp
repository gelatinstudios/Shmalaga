
#include <iostream>
#include <memory>
#include <vector>
#include <fstream>

#include <cmath>
#include <cstring>
#include <ctype.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "algorithms.hpp"
#include "winrend.hpp"
#include "bullets.hpp"
#include "assets.hpp"
#include "enemy.hpp"
#include "scores.hpp"
#include "boss.hpp"
#include "data.hpp"

int intro(Star[MAX_STARS], std::shared_ptr<SDL_Renderer>, std::shared_ptr<TTF_Font>);

bool handler(GameData &, std::shared_ptr<SDL_Window>, Sounds &);
void automata(GameData &, Sounds &);
void render(GameData &, std::shared_ptr<SDL_Renderer>, Assets &);

bool menu_handler(GameData &data, SDL_Event *event);
void render_menu(GameData &data, std::shared_ptr<SDL_Renderer> rend, std::shared_ptr<TTF_Font> font, std::shared_ptr<SDL_Texture> menu[]);
