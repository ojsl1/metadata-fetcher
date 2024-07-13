#ifndef INPUt_H
#define INPUT_H
#pragma once

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Mouse{
public:
  SDL_Surface *rawMouse = NULL;
  SDL_Surface *scaledMouse = NULL;
  SDL_Rect dRectMouse;
  SDL_Rect point;

  Mouse();
  void updateCursor();
  void drawCursor(SDL_Surface *gScreen);
};

#endif // INPUT_H
