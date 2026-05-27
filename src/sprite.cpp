#include "sprite.h"
#include "input.h" // Sprite::DetectClicks and Sprite::DetectCollisions depend on mouse class

SurfaceSprite::SurfaceSprite(const std::string &id, int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect)
  : srcRect(spriteRect), hasCollisions(false), toggled(false), rawSprite(nullptr), alternateSprite(nullptr), dRectSprite{x,y,w,h},
    x(x), y(y), name(id)
{
  if (!spritesheetPath){
    spritesheetPath = "assets/texture-error.png";
  }

  spritesheet = IMG_Load(spritesheetPath);
  if (!spritesheet){
    SDL_Log("Failed to load spritesheet: %s, SDL_Image Error: %s\n", spritesheetPath, IMG_GetError());
    spritesheet = IMG_Load("assets/texture-error.png");
    if (!spritesheet){
      SDL_Log("Failed to load fallback spritesheet: assets/texture-error.png, SDL_Image Error: %s\n", IMG_GetError());
      return;
    }
  }

  // Create rawSprite
  rawSprite = SDL_CreateRGBSurface(0, spriteRect.w, spriteRect.h, 32,
                                   spritesheet->format->Rmask, spritesheet->format->Gmask,
                                   spritesheet->format->Bmask, spritesheet->format->Amask);
  if(rawSprite){
    //Copy the portion of the spritesheet into rawSprite
    if (SDL_BlitSurface(spritesheet, &spriteRect, rawSprite, nullptr) < 0){
      SDL_Log("Failed to blit the extracted image from spritesheet: %s\n", SDL_GetError());
      SDL_FreeSurface(rawSprite);
      rawSprite = nullptr;
    }
  } else {
    SDL_Log("SDL_CreateRGBSurface failed at creating rawSprite from spritesheet: %s\n", SDL_GetError());
  }

  // Create alternateSprite
  SDL_Rect alternateRect = {spriteRect.x, (spriteRect.y + spriteRect.h), spriteRect.w, spriteRect.h};
  alternateSprite = SDL_CreateRGBSurface(0, alternateRect.w, alternateRect.h, 32,
                                         spritesheet->format->Rmask, spritesheet->format->Gmask,
                                         spritesheet->format->Bmask, spritesheet->format->Amask);
  if(alternateSprite){
    //Copy the alternate portion of the spritesheet into alternateSprite
    if (SDL_BlitSurface(spritesheet, &alternateRect, alternateSprite, nullptr) < 0){
      SDL_Log("Failed to blit the alternateSprite from spritesheet: %s\n", SDL_GetError());
      SDL_FreeSurface(alternateSprite);
      rawSprite = nullptr;
    }
  } else {
    SDL_Log("Failed to create alternateSprite: %s\n", SDL_GetError());
  }
}

SurfaceSprite::~SurfaceSprite(){
  if (rawSprite){
    std::cerr << "Freeing rawSprite [" << name << "] -> Address: " << rawSprite << std::endl;
    SDL_FreeSurface(rawSprite);
    rawSprite = nullptr;
  }
  if (spritesheet){
    std::cerr << "Freeing spritesheet [" << name << "] -> Address: " << spritesheet << std::endl;
    SDL_FreeSurface(spritesheet);
    spritesheet = nullptr;
  }
  if (alternateSprite){
    std::cerr << "Freeing alternateSprite [" << name << "] -> Address: " << alternateSprite << std::endl;
    SDL_FreeSurface(alternateSprite);
    alternateSprite = nullptr;
  }
}

void SurfaceSprite::Toggle(){
  toggled = !toggled;
  if (toggleCallback){
    toggleCallback(toggled);
  }
}

void SurfaceSprite::SetAlternateSprite(SDL_Surface *alternate){
  alternateSprite = alternate;
}

void SurfaceSprite::SetToggleCallback(std::function<void(bool)> callback){
  toggleCallback = callback;
}

void SurfaceSprite::DetectCollisions(Mouse &mouse){
  hasCollisions = SDL_HasIntersection(&dRectSprite, &mouse.point);
}
