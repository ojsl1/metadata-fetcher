#include "input.h"

Mouse::Mouse(const char* mouseImagePath)
  : rawMouse(NULL),
    scaledMouse(NULL)
{
    dRectMouse = {0,0,50,50}; // x,y,w,h,
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
    scaledMouse = SDL_CreateRGBSurface(0, 50, 50, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (!scaledMouse){
          SDL_Log("Failed to create scaled mouse surface, SDL Error: %s", SDL_GetError());
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
 * @param dRectMouse The only reason this helper function exists.
 * @details SDL_BlitSurface() somehow modifies its fourth argument, thus requiring it to be non-const.
 */
SDL_Rect Mouse::GetDrawRect() const {
  return SDL_Rect{ dRectMouse.x, dRectMouse.y, 50, 50 }; // Assuming width and height are constant
}

void Mouse::Update(){
      // Update the mouse's x and y position:
      SDL_GetMouseState(&dRectMouse.x, &dRectMouse.y);
      //TODO: Why are we pushing the private position values to a public member called point:
      point.x = dRectMouse.x;
      point.y = dRectMouse.y;
}

void Mouse::UpdateMouseState(SDL_Event &e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        std::cout << "click detected and clicked equals: " << clicked << std::endl;
        x = e.button.x;
        y = e.button.y;
        clicked = true;
    } else {
        std::cout << "click not detected and clicked equals: " << clicked << std::endl;
        clicked = false;
    }
}

Mouse::~Mouse(){
  if (rawMouse){
    SDL_FreeSurface(rawMouse);
  }

  if (scaledMouse){
    SDL_FreeSurface(scaledMouse);
  }
}
