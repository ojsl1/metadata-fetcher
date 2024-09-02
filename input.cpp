#include "input.h"

Mouse::Mouse(){
    rawMouse = IMG_Load("resources/mouse.png");
    // The size of the rectangle the cursor will be drawn on
    scaledMouse = SDL_CreateRGBSurface(0, 50, 50, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    // The size of the scaled cursor 
    dRectMouse =  { 0, 0, 64, 64 }; // x,y,w,h

    SDL_BlitScaled( rawMouse, NULL, scaledMouse, &dRectMouse );
    SDL_FreeSurface( rawMouse );

    point.w = 1;
    point.h = 1;
    SDL_ShowCursor(false);
}

Mouse::~Mouse(){
  // Clean up code for mouse, in what context though, is this necessary?
}

void Mouse::updateCursor(){
      SDL_GetMouseState(&dRectMouse.x, &dRectMouse.y);
      point.x = dRectMouse.x;
      point.y = dRectMouse.y;
}

void Mouse::drawCursor(SDL_Surface *gScreen){
    SDL_BlitSurface( scaledMouse, NULL, gScreen, &dRectMouse);
}
