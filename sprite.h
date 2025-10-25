#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"
#include <string>
#include <functional>

class Mouse; //Forward declare for Sprite::DetectCollisions

class Sprite
{
public:
  SDL_Rect srcRect;
  SDL_Surface *spritesheet;
  // x,y coords; w,h size; filepath;
  Sprite(const std::string &spriteName, int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect);
  ~Sprite();
  
  void virtual Draw(AppContext gApp);
  void DrawScaled(AppContext gApp);
  void Toggle();
  void DetectCollisions(Mouse &mouse);
  void SetToggleCallback(std::function<void(bool)> callback);
  void SetAlternateSprite(SDL_Surface *alternate);

  bool hascollisions; // WIP cant privatize main.cpp reads from this via playSprite.hasCollision
  bool toggled; // Current toggled state
  
  int getX() const { return dRectSprite.x; }
  int getY() const { return dRectSprite.y; }
  
protected:
  SDL_Surface *rawSprite;
  SDL_Surface *alternateSprite; // Alternate sprite for 2-state sprites
  SDL_Rect dRectSprite;
  int x, y; // origo topleft

private:
  std::string name;
  std::function<void(bool)> toggleCallback; // Callback for toggle action

};

#endif // SPRITE_H
