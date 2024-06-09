#ifndef MOUSE_H
#define MOUSE_H
#include "render.h"

class Mouse{
public:
  //SDL_Texture *imageMouse; 
  SDL_Surface *imageMouse;// 1. bruteforce changed from above SDL_Texture 2. not initializing to null causes a segfault in mouse.drawMouse() 
  SDL_Rect rectMouse;
  SDL_Rect pointMouse;

  Mouse(){
    imageMouse = IMG_Load("resources/mouse.png");
    rectMouse.w  = 25;
    rectMouse.h  = 25;
    pointMouse.w = 1; //point only for collision detectection
    pointMouse.h = 1; //which is why its only 1x1
    SDL_ShowCursor(false); //hide system cursor
  }

  void updateCursor(){
      SDL_GetMouseState(&rectMouse.x, &rectMouse.y);
      pointMouse.x = rectMouse.x; // collision point topleft
      pointMouse.y = rectMouse.y;
  }

  //void drawMouse(){
      //SDL_RenderCopy( ren, imageMouse, NULL, &rectMouse );
/* old method, not sure where destinationMouse was supposed to be
  void drawMouse( SDL_Surface *destinationMouse ){
      SDL_BlitSurface( imageMouse, NULL, destinationMouse, &rectMouse);
  }
*/
  void drawMouse( SDL_Surface *screen ){
      SDL_BlitSurface( imageMouse, NULL, screen, &rectMouse);
  }

};

#endif // MOUSE_H
