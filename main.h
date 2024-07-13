#ifndef MAIN_H
#define MAIN_H // GLOBALS.H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

extern SDL_Window *gWindow;
extern SDL_Surface *gScreen;

void initVideo(int window_width, int window_height);
void initMixer();

struct WindowDimensions{
    int wSize;
    int hSize;
    int xPosi;
    int yPosi;
};

// Extern Uint32 variables
extern Uint32 gPink;
extern Uint32 gRed;
extern Uint32 gBeige;
extern Uint32 gBlue;
extern Uint32 gDarkblue;
extern Uint32 gDarkgreen;

#endif // MAIN_H
