#ifndef MOUSE_H
#define MOUSE_H
#include "main.h" // globals
#include "render.h"
#include <iostream>

class Mouse{
public:
  SDL_Surface *imageMouse = NULL; // TODO: necessary to initialize as null?
  SDL_Rect rectMouse;
  SDL_Rect pointMouse;

  Mouse(){
    imageMouse = IMG_Load("resources/mouse.png");
    if (!imageMouse){
      std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    rectMouse.w  = 25;
    rectMouse.h  = 25;
    pointMouse.w = 1; //point only for collision detectection
    pointMouse.h = 1; //which is why its only 1x1
    SDL_ShowCursor(false); //hide system cursor
  }

  void updateCursor(){
      SDL_GetMouseState(&rectMouse.x, &rectMouse.y);
      pointMouse.x = rectMouse.x; // make collision point topleft
      pointMouse.y = rectMouse.y;
  }

  void drawMouse( SDL_Surface *screen ){
      SDL_BlitSurface( imageMouse, NULL, screen, &rectMouse);
  }

};

#endif // MOUSE_H
