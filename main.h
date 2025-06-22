#ifndef MAIN_H
#define MAIN_H // MAIN.H "GLOBALS.H"
#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#define FPSCAP 60
#define ALLEYS 1
#define DEBUG 0

#if ALLEYS
  #define WINDOW_WIDTH 320
  #define WINDOW_HEIGHT 480
#endif

struct WindowDimensions{
    int wSize;
    int hSize;
    int xPosi;
    int yPosi;
};

// Global declarations
extern SDL_Window *gWindow;
extern SDL_Surface *gScreen;
extern float fps;

extern Uint32 gPink;
extern Uint32 gRed;
extern Uint32 gBeige;
extern Uint32 gBlue;
extern Uint32 gDarkblue;
extern Uint32 gDarkgreen;

#endif // MAIN_H
