#ifndef MAIN_H
#define MAIN_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

extern SDL_Window *window;
extern SDL_Surface *screen;

void initVideo(int window_width, int window_height);
void initMixer();

struct WindowDimensions{
    int wSize;
    int hSize;
    int xPosi;
    int yPosi;
};

#endif // MAIN_H
