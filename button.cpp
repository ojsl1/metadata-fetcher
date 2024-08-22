#include "button.h"
#include "input.h" // Button::detectCursor depends on the Mouse class

Button::Button(int x, int y) : sRectButton{0,y,75,300}, dRectButton{x,y,75,300}{
    rawButton = IMG_Load("resources/button-inactive.png");
    selected = false;

    // Spreadsheet rectangles: TODO For spritesheets
    sRectButton.h = 100;  //each button has 100pixel height on the png
    sRectButton.w = 400;  //each button has 400pixel width on the png
    sRectButton.x = 0;    //always starting from the far left of the sprite sheet png,
                    //hence srect.y cant be set as it will change depending on the sprite
    
    /* Using initialization list (above)
     * Now when the Button is created in memory it will be created with those values
     * instead of garbage values that afterwards get replaced with the below values.
     * If instead using SDL_Texture, the initialization list is where you could scale the sRects
    //destination rects
    dRect.h = 75;
    dRect.w = 300;
    */
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

void Button::drawButton( SDL_Surface *gScreen){
    SDL_BlitSurface( rawButton, NULL, gScreen, &dRectButton );
}

void Button::updateButton(){

}

void Button::setXY(int x, int y){
  dRectButton.x = x;
  dRectButton.y = y;
}
