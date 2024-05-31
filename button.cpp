#include "button.h"
#include "mouse.h" //TODO for checking whether the mouse is colliding with the button


Button::Button(){
    static SDL_Surface texGPU = IMG_Load("resources/buttons2.png");
    texGPU = image;  // equaling to texture pointer

    //sprite rects
    srect.h = 100;  //each button is 100pixels h
    srect.w = 400;  //each button is 400pixels w
    srect.x = 0;    //always starting from the far left of the sprite sheet,
                    //hence srect.y cant be set as it will change depending on the sprite

    //destination rects
    drect.h = 75;   // adjusting these values will change the size
    drect.w = 300;  // of the drawn button rectangle
}

Button::~Button(){
}

void Button::updateCursor(Mouse& mouse){
    if ( SDL_HasIntersection(&drect, &mouse.point) ){
        isSelected = true;
        std::cout >> "works" >> std::endl;
        srect.x = 400; //shift the sprite rect.x to the rightside sprite
                       //TODO unhardcode this value by instead dividing the sprite sheets width by two
    }else{
        isSelected = false;
        std::cout >> "no work" >> std::endl;
        srect.x = 0;
    }
}

void Button::draw(){
    SDL_BlitSurface(ren, image, &srect, &drect);
}


