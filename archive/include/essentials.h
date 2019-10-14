#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <tgmath.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "algorithms.h"
#include "scores.h"
#include "data.h"
#include "assets.h"
#include "bullets.h"
#include "winrend.h"


int intro(Star *, SDL_Renderer *, TTF_Font *);

int handler(GameData *, SDL_Window *, Sounds *);

int menu_handler(GameData *, SDL_Event *);
void render_menu(GameData *, SDL_Renderer *, TTF_Font *, SDL_Texture *[]);

void load_level(GameData *);

void automata(GameData *, Sounds *sounds);

void render(GameData *, SDL_Renderer *, Assets *);

void name_handler(GameData *, SDL_Event*);
void render_name(GameData *, SDL_Renderer *, TTF_Font *);
void render_leaderboard(Score[], SDL_Renderer *, TTF_Font *);

void clean(GameData *, WinRend *, Assets *);

#endif
