#ifndef SPRITE_H
#define SPRITE_H

class Mouse; // forward declaring Mouse class

#include "main.h" // globals
#include "render.h"
#include <iostream>
#include <functional>

class Sprite{
public:
  // x,y coords (origo is topleft); w,h size, filepath
  Sprite(int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect);
  ~Sprite();
  
  void Draw(SDL_Surface *gScreen);
  void DrawScaled(SDL_Surface *gScreen);
  void Toggle(); // Toggles between states
  void DetectIntersections(Mouse &mouse); // Using the forward declared Mouse class
  void SetToggleCallback(std::function<void(bool)> callback); // Set a callback for toggle actions
  void SetAlternateSprite(SDL_Surface *alternate); // Set the alternate surface

  bool hasintersection; // WIP cant privatize main.cpp reads from this via playSprite.hasintersection
  bool toggled; // Current toggled state

private:
  SDL_Surface *rawSprite; // Specific image extracted from the spritesheet
  SDL_Surface *alternateSprite; // Alternate image for toggled states
  SDL_Surface *spritesheet;
  SDL_Rect dRectSprite; // The specific images position and size
  std::function<void(bool)> toggleCallback; // Callback for toggle action
};
#endif // SPRITE_H
