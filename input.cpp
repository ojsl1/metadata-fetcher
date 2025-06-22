#include "input.h"

Mouse::Mouse(int w, int h, const char* mouseImagePath)
  : rawMouse(nullptr),scaledMouse(nullptr)
{
    // For storing the dimensions of the scaled cursor
    //TODO DEBUG for non-zero x,y values
    dRectMouse = {0,0,w,h}; // x,y,w,h

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
      scaledMouse = nullptr;
      return;
    }

    // Scale the rawMouse on the scaledMouse surface
    SDL_BlitScaled(rawMouse, nullptr, scaledMouse, &dRectMouse);
    
    SDL_ShowCursor(SDL_DISABLE);
}

void Mouse::Draw(SDL_Surface *gScreen) const {
    SDL_Rect finalRect = GetDrawRect();
    SDL_BlitSurface(scaledMouse, nullptr, gScreen, &finalRect);
}

/**
 * @brief Convert const SDL_Rect into a non-const SDL_Rect.
 * @details SDL_BlitSurface somehow modifies its fourth argument,
 * thus requiring it to be non-const. This is only used by dRectMouse.
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
    std::cerr << "Freeing rawMouse [] -> Address: " << rawMouse << std::endl;
    SDL_FreeSurface(rawMouse);
    rawMouse = nullptr;
  }
  if (scaledMouse){
    std::cerr << "Freeing scaledMouse [] -> Address: " << scaledMouse << std::endl;
    SDL_FreeSurface(scaledMouse);
    scaledMouse = nullptr;
  }
}
