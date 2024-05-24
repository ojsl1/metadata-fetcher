#ifndef MOUSE_H
#define MOUSE_H
#include "render.h"

class Mouse{
public:
  //SDL_Texture *image;
  SDL_Surface *image;
  SDL_Rect rect;
  SDL_Rect point;

  Mouse(){
    image = IMG_Load("resources/mouse.png");
    rect.w  = 25;
    rect.h  = 25;
    point.w = 1; //point only for collision detectection
    point.h = 1; //which is why its only 1x1
    SDL_ShowCursor(false); //hide system cursor
  }

  void updateCursor(){
      SDL_GetMouseState(&rect.x, &rect.y);
      point.x = rect.x; // collision point topleft
      point.y = rect.y;
  }

  void drawMouse(){
      //SDL_RenderCopy( ren, image, NULL, &rect );
  }

};
#endif // MOUSE_H
