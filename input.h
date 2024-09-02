#ifndef INPUt_H
#define INPUT_H
#pragma once

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Mouse{
private:
  SDL_Surface *rawMouse = NULL;
  SDL_Surface *scaledMouse = NULL;
  SDL_Rect dRectMouse;

public:
  SDL_Rect point; //TODO button::detectCursor() reads from this
                  //
  Mouse();
  ~Mouse();
  
  // Getter for the private point
  SDL_Rect getPoint() const {
    return point;
  }

  void updateCursor();
  void drawCursor(SDL_Surface *gScreen);
};

#endif // INPUT_H
