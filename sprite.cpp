#include "sprite.h"
#include "input.h" // Sprite::DetectClicks and Sprite::DetectIntersections depend on mouse class

Sprite::Sprite(int x, int y, int w, int h, const char* spritesheetPath, SDL_Rect spriteRect)
  : rawSprite(nullptr), hasintersection(false), toggled(false)
    // TODO Without this init list the sprites gets drawn at topleft, why?
{
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

    // Extract the specific image from the spritesheet
    rawSprite = SDL_CreateRGBSurface(0, spriteRect.w, spriteRect.h, 32,
                                     spritesheet->format->Rmask, spritesheet->format->Gmask,
                                     spritesheet->format->Bmask, spritesheet->format->Amask);
    if(!rawSprite){
      SDL_Log("SDL_CreateRGBSurface failed at creating image from spritesheet: ", SDL_GetError());
      return;
    }

    //Blit the portion of the spritesheet into the rawSprite
    if (SDL_BlitSurface(spritesheet, &spriteRect, rawSprite, NULL) < 0){
      SDL_Log("Failed to blit the extracted image from spritesheet: %s\n", SDL_GetError());
      SDL_FreeSurface(rawSprite);
      rawSprite = nullptr;
    }
}

Sprite::~Sprite(){
    if (rawSprite){
      SDL_FreeSurface(rawSprite);
    }
    if (spritesheet){
      SDL_FreeSurface(spritesheet);
    }
}

void Sprite::DetectIntersections(Mouse &mouse){
    hasintersection = SDL_HasIntersection(&dRectSprite, &mouse.point);
}

void Sprite::Draw(SDL_Surface *gScreen){
    if (rawSprite){
      SDL_BlitSurface(rawSprite, NULL, gScreen, &dRectSprite);
    }
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
    if (SDL_BlitScaled(rawSprite, NULL, scaledSpriteLocal, NULL) < 0){
        // NB: scaledSpriteLocal is 32bpp, the images have to also be 32bpp.
        SDL_Log("SDL_BlitScaled failed: %s\n", SDL_GetError()); // e.g. "Blit combination not supported"
        SDL_FreeSurface(scaledSpriteLocal);
        return;
    }

    // Blit scaledSpriteLocal onto the screen.
    if (SDL_BlitSurface(scaledSpriteLocal, NULL, gScreen, &dRectSprite) < 0){
        std::cout << "rawSprite BitsPerPixel: " << rawSprite->format->BitsPerPixel << std::endl;
        std::cout << "scaledSprite BitsPerPixel: " << scaledSpriteLocal->format->BitsPerPixel << std::endl;
        SDL_Log("SDL_BlitSurface failed: %s\n", SDL_GetError());
    }

    // Free the local surface
    SDL_FreeSurface(scaledSpriteLocal);
}
