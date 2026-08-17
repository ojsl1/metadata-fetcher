#ifndef SPRITE_H
#define SPRITE_H

#include "main.h"
#include <string>
#include <functional>

class Mouse; //Forward declare for Sprite::DetectCollisions

class ISprite {
public:
  // Non-SDL interface
  virtual ~ISprite() = default;

  // @brief getters
  virtual int getX() const = 0;
  virtual int getY() const = 0;
};

class SurfaceSprite : public ISprite {
public:
  // SDL_Surface sprite

  SDL_Rect srcRect;
  SDL_Surface *spritesheet;
  // @brief Track sprites that have collisions
  bool hasCollisions;
  // @brief Current toggled state
  bool toggled;
  SDL_Surface *rawSprite;
  SDL_Surface *alternateSprite; // Alternate sprite for 2-state sprites
  SDL_Rect dRectSprite;

  /** @param
   * - std::string - identifier
   * - x,y = drawing coordinates
   * - w,h = drawing size
   * - {x,y,w,h} coordinates and size on the spriteRect of the source spritesheetPath
   *  @note Only DrawScaled() uses the drawing size values
   */
  SurfaceSprite(const std::string &id, int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect);
  ~SurfaceSprite() override;

  // @brief getters
  int getX() const override { return dRectSprite.x; }
  int getY() const override { return dRectSprite.y; }
  virtual SDL_Surface *GetDrawSurface() const;
  virtual const SDL_Rect *GetSourceRect() const;

  /* TODO move these outta here */
  // @brief Toggle between states.
  void Toggle();
  void DetectCollisions(Mouse &mouse);
  // @brief Set a callback for toggle actions. 
  void SetToggleCallback(std::function<void(bool)> callback);
  // @brief Set the alternate surface.
  void SetAlternateSprite(SDL_Surface *alternate);

  int x, y; // origo topleft

  std::string name;
  std::function<void(bool)> toggleCallback; // Callback for toggle action
};

class TextureSprite : public ISprite {
public:
  // SDL_Texture
};

class GLSprite : public ISprite {
public:
  // OpenGL sprite 
};

class VKSprite : public ISprite {
public:
  // Vulkan sprite
};

#endif // SPRITE_H
