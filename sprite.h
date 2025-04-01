#ifndef SPRITE_H
#define SPRITE_H

#include "main.h" // globals
#include "render.h"
#include <iostream>
#include <functional>

class Mouse; //Forward declare for Sprite::DetectIntersections


class Sprite{
private:
  std::string name;
  std::function<void(bool)> toggleCallback; // Callback for toggle action

protected:
  SDL_Surface *spritesheet;
  SDL_Surface *rawSprite; // Specific image extracted from the spritesheet
  SDL_Surface *alternateSprite; // Alternate image for toggled states
  SDL_Rect dRectSprite; // The specific images position and size
  int x, y;             // origo topleft; public so character class can get its coordinates

public:
  // x,y coords; w,h size; filepath;
  Sprite(const std::string &spriteName, int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect);
  ~Sprite();
  
  void virtual Draw(SDL_Surface *gScreen);
  void DrawScaled(SDL_Surface *gScreen);
  void Toggle(); // Toggles between states
  void DetectIntersections(Mouse &mouse); // Using the forward declared Mouse class
  void SetToggleCallback(std::function<void(bool)> callback); // Set a callback for toggle actions
  void SetAlternateSprite(SDL_Surface *alternate); // Set the alternate surface

  bool hasintersection; // WIP cant privatize main.cpp reads from this via playSprite.hasintersection
  bool toggled; // Current toggled state
  
  int getX() const { return dRectSprite.x; }
  int getY() const { return dRectSprite.y; }

};

#endif // SPRITE_H
