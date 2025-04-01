#ifndef CHARACTER_H
#define CHARACTER_H

#include "main.h" // globals
#include "sprite.h"

class Character : public Sprite{
private:
  SDL_Rect srcRect;

public:
  int frameWidth, frameHeight;  // dimensions of a single frame
  int frameCount;               // total frames in animation
  int framePadding;             // padding between frames (1px)
  int currentFrame;             // Current animation frame
  Uint32 animationSpeed;        // speed of animation (in ms)
  Uint32 lastUpdate;            // time since last frame update
  int speed;                    // movement speed

  Character(const std::string &spriteName, int x, int y, int w, int h,
            const char *spritesheetPath, SDL_Rect spriteRect, int totalFrames);

  void update(double deltaTime);// update animation
  void move(int dx, int dy);    // handle movement
  void slash(); // change to attack animation
  void Draw(SDL_Surface *gScreen) override;

};

#endif // CHARACTER_H
