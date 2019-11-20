#ifndef ESSENTIALS_H
#define ESSENTIALS_H

//#define NDEBUG

#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <tgmath.h>
#include <assert.h>

#ifdef SHMALAGA_DEBUG
#include <sched.h>

static inline size_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((size_t)hi << 32) | lo;
}

size_t cycles;

#define CNT_START (cycles = rdtsc())
// #define CNT_RESTART (cycles = rdstc())
#define CNT_PRINT(x) (printf("%s: clock cycles: %zu\n", (x), rdtsc() - cycles))

#else

#define CNT_START
#define CNT_PRINT(x) {}

#endif

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

int menu_handler(GameData *, Mix_Chunk *[], SDL_Event *);
void render_menu(GameData *, SDL_Renderer *, TTF_Font *, SDL_Texture *[]);

void load_level(GameData *);

void automata(GameData *, Sounds *sounds);

void update(GameData *data, SDL_Renderer *rend, Sounds *sounds, SDL_Texture *score_text, TTF_Font *font);

void render(GameData *, SDL_Renderer *, Assets *);

void name_handler(GameData *, SDL_Event*);
void render_name(GameData *, SDL_Renderer *, SDL_Texture *[], TTF_Font *);
void render_leaderboard(Score[], SDL_Renderer *, SDL_Texture *[], TTF_Font *, size_t);

void clean(WinRend *, Assets *);

#endif
