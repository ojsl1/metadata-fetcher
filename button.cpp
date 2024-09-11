#include "button.h"
#include "input.h" // Button::detectCursor depends on the Mouse class

Button::Button(int x, int y, const char* buttonImagePath)
  : sRectButton{0,y,75,300},          // Initialization list
    dRectButton{x,y,75,300},          // Member variables are initialized directly
    rawButton(NULL), selected(false)  // Default init values
                                      // TODO Without this init list the buttons gets drawn at topleft, why?
{
    rawButton = IMG_Load(buttonImagePath);
    if (!rawButton){
      SDL_Log("Failed to load image: %s, SDL_Image Error: %s", buttonImagePath, IMG_GetError());
    }
#if 0
    // WIP: Reading rects from a spritesheet:
    sRectButton.h = 100;  //each button has 100pixel height on the png spritesheet
    sRectButton.w = 400;  //each button has 400pixel width on the png spritesheet

    sRectButton.x = 0;    //to always start from the far left of the sprite sheet png,
                    //hence srect.y cant be set as it will change depending on the sprite

    //destination rects
    dRect.h = 75;
    dRect.w = 300;
    */
#endif
}

Button::~Button(){
  if (rawButton){
    SDL_FreeSurface(rawButton);
  }
}

void Button::detectCursor( Mouse &mouse){
      if ( SDL_HasIntersection(&dRectButton, &mouse.point) ){
          selected = true;
          std::cout << "hasintersection" << std::endl;
          //sRect.x = 400;  //TODO For spritesheets
                            //with sRect.w set as 400 this shifts the x to the rightside sprite
                            //TODO unhardcode this value by dividing Spritesheet width the amount of sprite columns
      }else{
          selected = false;
          //srect.x = 0; //TODO For spritesheets
      }
}

void Button::drawButton(SDL_Surface *gScreen){
    SDL_BlitSurface(rawButton, NULL, gScreen, &dRectButton);
}

#if 0 // WIP this is for updating buttons eg. on mousehover
void Button::updateButton(){
}
#endif

#if 0 // WIP
void Button::setXY(int x, int y){
  dRectButton.x = x;
  dRectButton.y = y;
}
#endif
