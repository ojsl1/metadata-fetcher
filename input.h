#ifndef INPUT_H
#define INPUT_H
#pragma once

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Mouse{
public:
  // Optional filepath for custom cursor
  Mouse(const char* mouseImagePath = NULL);
  ~Mouse();
  
  SDL_Rect point; // WIP cant privatize button.cpp reads from this via button::detectCursor()
  
  // WIP to privatize point, use getter for getting the values
  SDL_Rect getPoint() const {
    return point;
  }

  void updateCursor();
  void drawCursor(SDL_Surface *gScreen);

private:
  SDL_Surface *rawMouse = NULL;
  SDL_Surface *scaledMouse = NULL;
  SDL_Rect dRectMouse; // The size of the scaled cursor
};

#endif // INPUT_H
