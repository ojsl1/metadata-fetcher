#ifndef BUTTON_H
#define BUTTON_H

class Mouse; // forward declaring Mouse class

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Button{
public:
  // x,y coords (origo is topleft); w,h size, filepath
  Button(int x, int y, int width, int height, const char* imagePath);
  ~Button();
  
  bool hasintersection; // WIP cant privatize main.cpp reads from this via playButton.hasintersection
  bool toggled;

  void DetectIntersections(Mouse &mouse); // Using the forward declared Mouse class
  void DetectClicks(Mouse &mouse);

  void Draw(SDL_Surface *gScreen);
  void DrawScaled(SDL_Surface *gScreen);

private:
  SDL_Surface *rawButton = NULL;
  SDL_Surface *scaledButton;
  SDL_Rect dRectButton;
};

#endif // BUTTON_H
