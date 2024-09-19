#include "input.h"

Mouse::Mouse(const char* mouseImagePath)
  : rawMouse(NULL),
    scaledMouse(NULL)
{
    dRectMouse = {0,0,64,64}; // x,y,w,h,
    point = {0,0,1,1}; // x,y,w,h

    if (!mouseImagePath){
      mouseImagePath = "resources/mouse-fallback.png";
    }

    rawMouse = IMG_Load(mouseImagePath);
    if (!rawMouse ){
      SDL_Log("Failed to load image: %s, SDL_Image Error: %s", mouseImagePath, IMG_GetError());
      rawMouse = IMG_Load("resources/mouse-fallback.png");
      if (!rawMouse){
        SDL_Log("Failed to load fallback image: resources/mouse-fallback.png, SDL_Image Error: %s", IMG_GetError());
        return;
      }
    }

    // The size of the rectangle the cursor will be drawn on
    scaledMouse = SDL_CreateRGBSurface(0, 50, 50, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!scaledMouse){
      SDL_Log("Failed to create scaled mouse surface, SDL Error: %s", SDL_GetError());
      return;
    }

    // Scale the rawMouse on the scaledMouse surface
    SDL_BlitScaled(rawMouse, NULL, scaledMouse, &dRectMouse);
    
    SDL_ShowCursor(SDL_DISABLE);
}

Mouse::~Mouse(){
  if (rawMouse){
    SDL_FreeSurface(rawMouse);
  }

  if (scaledMouse){
    SDL_FreeSurface(scaledMouse);
  }
}

void Mouse::Update(){
      SDL_GetMouseState(&dRectMouse.x, &dRectMouse.y);
      point.x = dRectMouse.x;
      point.y = dRectMouse.y;
}

void Mouse::Draw(SDL_Surface *gScreen){
    SDL_BlitSurface(scaledMouse, NULL, gScreen, &dRectMouse);
}
