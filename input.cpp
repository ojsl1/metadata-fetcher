#include "input.h"

Mouse::Mouse(int w, int h, const char* mouseImagePath)
  : rawMouse(NULL),scaledMouse(NULL)
{
    dRectMouse = {0,0,w,h}; // x,y,w,h //TODO add debugging for non-zero x,y values
    point = {0,0,1,1}; // x,y,w,h

    if (!mouseImagePath){
      mouseImagePath = "assets/mouse-fallback.png";
    }

    rawMouse = IMG_Load(mouseImagePath);
        if (!rawMouse ){
          SDL_Log("Failed to load image: %s, SDL_Image Error: %s", mouseImagePath, IMG_GetError());
          rawMouse = IMG_Load("assets/mouse-fallback.png");
          if (!rawMouse){
            SDL_Log("Failed to load fallback image: assets/mouse-fallback.png, SDL_Image Error: %s", IMG_GetError());
            return;
          }
        }

    // The size of the rectangle the cursor will be drawn on
    scaledMouse = SDL_CreateRGBSurface(
        rawMouse->flags,
        dRectMouse.w,
        dRectMouse.h,
        32,
        rawMouse->format->Rmask,
        rawMouse->format->Gmask,
        rawMouse->format->Bmask,
        rawMouse->format->Amask);

    if (!scaledMouse){
      SDL_Log("SDL_CreateRGBSurface failed: %s", SDL_GetError());
      return;
    }

    // Scale the rawMouse on the scaledMouse surface
    SDL_BlitScaled(rawMouse, NULL, scaledMouse, &dRectMouse);
    
    SDL_ShowCursor(SDL_DISABLE);
}

void Mouse::Draw(SDL_Surface *gScreen) const {
    SDL_Rect finalRect = GetDrawRect();
    SDL_BlitSurface(scaledMouse, NULL, gScreen, &finalRect);
}

/**
 * @brief Convert const SDL_Rect into a non-const SDL_Rect.
 * @param dRectMouse is the only reason this helper function exists.
 * @details SDL_BlitSurface somehow modifies its fourth argument, thus requiring it to be non-const.
 */
SDL_Rect Mouse::GetDrawRect() const {
  return SDL_Rect{ dRectMouse.x, dRectMouse.y, dRectMouse.w, dRectMouse.h };
}

void Mouse::GetXY(){
      // Getter for the mouse x and y positions
      SDL_GetMouseState(&dRectMouse.x, &dRectMouse.y);

      point.x = dRectMouse.x;
      point.y = dRectMouse.y;
}

Mouse::~Mouse(){
  if (rawMouse){
    SDL_FreeSurface(rawMouse);
  }

  if (scaledMouse){
    SDL_FreeSurface(scaledMouse);
  }
}
