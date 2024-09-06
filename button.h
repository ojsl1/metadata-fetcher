#ifndef BUTTON_H
#define BUTTON_H

// Forward declaration of Mouse class
class Mouse;

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Button{
public:
  SDL_Surface *rawButton = NULL;
  SDL_Rect sRectButton; // TODO For spritesheets
  SDL_Rect dRectButton;
  bool selected;

  // pass in the x and y of the topleft corner of the sprite
  Button(int x, int y);
  ~Button();

  // Method declaration using the forward declared Mouse class
  void detectCursor(Mouse &mouse);

  void drawButton(SDL_Surface *gScreen);

#if 0 // WIP see definition in button.cpp
  void updateButton();
#endif

  // set the destination
  void setXY(int x, int y);
};

#endif // BUTTON_H
