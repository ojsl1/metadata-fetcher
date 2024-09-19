#ifndef BUTTON_H
#define BUTTON_H

class Mouse; // forward declaring Mouse class

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Button{
public:
  // Image topleft x,y coords, size of rect, and filepath
  Button(int x, int y, int width, int height, const char* imagePath);
  ~Button();
  
  bool selected; // WIP cant privatize main.cpp reads from this via playButton.selected

  // Using the forward declared Mouse class
  void DetectCollisions(Mouse &mouse);

  void Draw(SDL_Surface *gScreen);
  void DrawScaled(SDL_Surface *gScreen);

private:
  SDL_Surface *rawButton = NULL;
  SDL_Surface *scaledButton = NULL;
  SDL_Rect dRectButton;
};

#endif // BUTTON_H
