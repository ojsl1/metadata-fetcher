#include "sprite.h"
#include "input.h" // Sprite::DetectClicks and Sprite::DetectIntersections depend on mouse class

Sprite::Sprite(const std::string &spriteName, int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect)
  : name(spriteName), rawSprite(nullptr), alternateSprite(nullptr), x(x), y(y), hasintersection(false), toggled(false){
  dRectSprite = {x,y,w,h};

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

Sprite::~Sprite(){
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

void Sprite::Toggle(){
  toggled = !toggled;
  if (toggleCallback){
    toggleCallback(toggled);
  }
}

void Sprite::SetAlternateSprite(SDL_Surface *alternate){
  alternateSprite = alternate;
}

void Sprite::SetToggleCallback(std::function<void(bool)> callback){
  toggleCallback = callback;
}

void Sprite::DetectIntersections(Mouse &mouse){
  hasintersection = SDL_HasIntersection(&dRectSprite, &mouse.point);
}

void Sprite::Draw(SDL_Surface *gScreen){
  SDL_Surface *currentSprite = toggled && alternateSprite ? alternateSprite : rawSprite;
  if (!currentSprite || !gScreen) return;
  SDL_BlitSurface(currentSprite, nullptr, gScreen, &dRectSprite);  
}

void Sprite::DrawScaled(SDL_Surface *gScreen){
  if (!rawSprite || !gScreen){
      SDL_Log("Invalid input surface (rawSprite or gScreen is null)\n");
      return;
  }

  #if DEBUG
  // Log and check rawSprite bpp.
  SDL_Log("rawSprite BitsPerPixel: %d\n", rawSprite->format->BitsPerPixel);
  if (rawSprite->format->BitsPerPixel != 32 &&
      rawSprite->format->BitsPerPixel != 16 &&
      rawSprite->format->BitsPerPixel != 8) {
      SDL_Log("Unsupported rawSprite format: %d bpp\n", rawSprite->format->BitsPerPixel);
      return;
  }
  // Log rawSprite pixel format details.
  SDL_Log("rawSprite Pixel Format: Rmask: 0x%X, Gmask: 0x%X, Bmask: 0x%X, Amask: 0x%X\n",
          rawSprite->format->Rmask,
          rawSprite->format->Gmask,
          rawSprite->format->Bmask,
          rawSprite->format->Amask);
  #endif

  // Create a 32bpp scaled surface
  SDL_Surface *scaledSpriteLocal = SDL_CreateRGBSurface(
      rawSprite->flags,
      dRectSprite.w,
      dRectSprite.h,
      32,
      rawSprite->format->Rmask,
      rawSprite->format->Gmask,
      rawSprite->format->Bmask,
      rawSprite->format->Amask);

  if (!scaledSpriteLocal){
    SDL_Log("SDL_CreateRGBSurface failed: %s\n", SDL_GetError()); // e.g. "Unknown pixel format"
    return;
  }

  /**
   * @brief Create a local destination rectangle for custom scaling
   * @param Replace the fourth arg of SDL_BlitScaled with &scaleRect
   * @details If you want to scale only a portion of the rawSprite or
   * scale it to a specific region of the destination surface, e.g. if
   * scaleRect.w and scaleRect.h differ from scaledSprite.
   * @notes If the entire rawSprite is scaled to the exact dimensions of
   * scaledSpriteLocal, you don't need scaleRect. SDL_BlitScaled automatically matches
   * the size of the source surface to the destination surface if NULL is passed.
   */
   //SDL_Rect scaleRect = {0, 0, dRectSprite.w, dRectSprite.h}; // x,y,w,h

  // Scale rawSprite on scaledSpriteLocal.
  if (SDL_BlitScaled(rawSprite, nullptr, scaledSpriteLocal, nullptr) < 0){
      // NB: scaledSpriteLocal is 32bpp, the images have to also be 32bpp.
      SDL_Log("SDL_BlitScaled failed: %s\n", SDL_GetError()); // e.g. "Blit combination not supported"
      SDL_FreeSurface(scaledSpriteLocal);
      return;
  }

  // Blit scaledSpriteLocal onto the screen.
  if (SDL_BlitSurface(scaledSpriteLocal, nullptr, gScreen, &dRectSprite) < 0){
      std::cout << "rawSprite BitsPerPixel: " << rawSprite->format->BitsPerPixel << std::endl;
      std::cout << "scaledSprite BitsPerPixel: " << scaledSpriteLocal->format->BitsPerPixel << std::endl;
      SDL_Log("SDL_BlitSurface failed: %s\n", SDL_GetError());
  }

  // Free the local surface
  SDL_FreeSurface(scaledSpriteLocal);
}
