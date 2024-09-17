#ifndef BUTTON_H
#define BUTTON_H

// Forward declaration of Mouse class
class Mouse;

#include "main.h" // globals
#include "render.h"
#include <iostream>

class Button{
public:
  // Topleft x,y coords of the image and filepath
  Button(int x, int y, int width, int height, const char* imagePath);
  ~Button();
  
  bool selected; // WIP cant privatize main.cpp reads from this via playButton.selected

  // Using the forward declared Mouse class
  void detectCursor(Mouse &mouse);

  void drawButton(SDL_Surface *gScreen);
  void drawButtonScaled(SDL_Surface *gScreen);

#if 0 // WIP see definition in button.cpp
  void updateButton();
#endif

  // WIP Setter for the destination
  void setXY(int x, int y);

private:
  SDL_Surface *rawButton = NULL;
  SDL_Surface *scaledButton = NULL;
  SDL_Rect sRectButton; // WIP For spritesheets
  SDL_Rect dRectButton;
};

#endif // BUTTON_H
