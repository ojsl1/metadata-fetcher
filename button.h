#ifndef BUTTON_H
#define BUTTON_H

class Mouse; // forward declaring Mouse class

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Button{
public:
  // x,y coords (origo is topleft); w,h size, filepath
  Button(int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect);
  ~Button();
  
  bool hasintersection; // WIP cant privatize main.cpp reads from this via playButton.hasintersection
  bool toggled;

  void DetectIntersections(Mouse &mouse); // Using the forward declared Mouse class

  void Draw(SDL_Surface *gScreen);
  void DrawScaled(SDL_Surface *gScreen);

private:
  SDL_Surface *spritesheet;
  SDL_Surface *rawButton; // Specific image extracted from the spritesheet
  SDL_Rect dRectButton; // The specific images position and size
};

#endif // BUTTON_H
