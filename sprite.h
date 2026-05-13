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

/** @param
 * - std::string - identifier
 * - x,y = drawing coordinates
 * - w,h = drawing size
 * - {x,y,w,h} coordinates and size on the spriteRect of the source spritesheetPath
 *  @warning Only DrawScaled() can use the drawing size values
 */
  Sprite(const std::string &id, int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect);
  ~Sprite();
  
/** @brief Draw sprites
 *  @details Uses SDL_BlitSurface()
 *  @note Performs a terniary check against alternateSprite for buttons
 *  @deprecated Every draw call should use DrawScaled()
 */
  void virtual Draw(AppContext gApp);
/** @brief Draw scaled sprites
 *  @details Uses SDL_BlitScaled()->SDL_BlitSurface()
 */
  void DrawScaled(AppContext gApp);
/* @brief Toggle between states.
 */
  void Toggle();
  void DetectCollisions(Mouse &mouse);
/**
 * @brief Set a callback for toggle actions.
 */
  void SetToggleCallback(std::function<void(bool)> callback);
/**
  * @brief Set the alternate surface.
  */
  void SetAlternateSprite(SDL_Surface *alternate);


/**
  * @brief Track sprites that have collisions
  * @todo cant privatize as main.cpp reads from this via playSprite.hasCollisions()
  */
  bool hasCollisions;

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
