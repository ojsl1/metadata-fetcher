#include "button.h"
#include "input.h" // Button::DetectClicks and Button::DetectIntersections depend on mouse class

Button::Button(int x, int y, int w, int h, const char* buttonImagePath)
  : rawButton(NULL),
    scaledButton(NULL),
    hasintersection(false),
    toggled(false)
    // TODO Without this init list the buttons gets drawn at topleft, why?
{
    dRectButton = {x,y,w,h};

    if (!buttonImagePath){
      buttonImagePath = "assets/texture-error.png";
    }

    rawButton = IMG_Load(buttonImagePath);
    if (!rawButton){
      SDL_Log("Failed to load image: %s, SDL_Image Error: %s\n", buttonImagePath, IMG_GetError());
      rawButton = IMG_Load("assets/texture-error.png");
      if (!rawButton){
        SDL_Log("Failed to load fallback image: assets/texture-error.png, SDL_Image Error: %s\n", IMG_GetError());
        return;
      }
    }
}

Button::~Button(){
  if (rawButton){
    SDL_FreeSurface(rawButton);
  }
}

void Button::DetectIntersections(Mouse &mouse){
    if ( SDL_HasIntersection(&dRectButton, &mouse.point) ){
        hasintersection = true;
        //std::cout << "hasintersection" << std::endl;
    }else{
        hasintersection = false;
    }
}

void Button::DetectClicks(Mouse &mouse){
    if (mouse.clicked &&
        mouse.x >= dRectButton.x &&
        mouse.x <= (dRectButton.x + dRectButton.w) &&
        mouse.y >= dRectButton.y &&
        mouse.y <= (dRectButton.y + dRectButton.h)) {

        toggled = !toggled;
        std::cout << "toggled state is: " << toggled << std::endl;
        }
}

void Button::Draw(SDL_Surface *gScreen){
    SDL_BlitSurface(rawButton, NULL, gScreen, &dRectButton);
}

void Button::DrawScaled(SDL_Surface *gScreen){
    if (!rawButton || !gScreen){
        SDL_Log("Invalid input surface (rawButton or gScreen is null)\n");
        return;
    }

    #if DEBUG
    // Log and check rawButton bpp.
    SDL_Log("rawButton BitsPerPixel: %d\n", rawButton->format->BitsPerPixel);
    if (rawButton->format->BitsPerPixel != 32 &&
        rawButton->format->BitsPerPixel != 16 &&
        rawButton->format->BitsPerPixel != 8) {
        SDL_Log("Unsupported rawButton format: %d bpp\n", rawButton->format->BitsPerPixel);
        return;
    }
    // Log rawButton pixel format details.
    SDL_Log("rawButton Pixel Format: Rmask: 0x%X, Gmask: 0x%X, Bmask: 0x%X, Amask: 0x%X\n",
            rawButton->format->Rmask,
            rawButton->format->Gmask,
            rawButton->format->Bmask,
            rawButton->format->Amask);
    #endif


    // Create a 32bpp scaled surface
    SDL_Surface *scaledButton = SDL_CreateRGBSurface(
        rawButton->flags,
        dRectButton.w,
        dRectButton.h,
        32,
        rawButton->format->Rmask,
        rawButton->format->Gmask,
        rawButton->format->Bmask,
        rawButton->format->Amask);

    if (!scaledButton){
      SDL_Log("SDL_CreateRGBSurface failed: %s\n", SDL_GetError()); // e.g. "Unknown pixel format"
      return;
    }

    /**
     * @brief Create a local destination rectangle for custom scaling
     * @param Replace the fourth arg of SDL_BlitScaled with &scaleRect
     * @details If you want to scale only a portion of the rawButton or
     * scale it to a specific region of the destination surface, e.g. if
     * scaleRect.w and scaleRect.h differ from scaledButton.
     * @notes If the entire rawButton is scaled to the exact dimensions of
     * scaledButton, you don't need scaleRect. SDL_BlitScaled automatically matches
     * the size of the source surface to the destination surface if NULL is passed.
     */
     //SDL_Rect scaleRect = {0, 0, dRectButton.w, dRectButton.h}; // x,y,w,h

    // Scale rawButton on scaledButton.
    if (SDL_BlitScaled(rawButton, NULL, scaledButton, NULL) < 0){
        // NB: scaledButton is 32bpp, the images have to also be 32bpp.
        SDL_Log("SDL_BlitScaled failed: %s\n", SDL_GetError()); // e.g. "Blit combination not supported"
        SDL_FreeSurface(scaledButton);
        return;
    }

    // Blit scaledButton onto the screen.
    if (SDL_BlitSurface(scaledButton, NULL, gScreen, &dRectButton) < 0){
        std::cout << "rawButton BitsPerPixel: " << rawButton->format->BitsPerPixel << std::endl;
        std::cout << "scaledButton BitsPerPixel: " << scaledButton->format->BitsPerPixel << std::endl;
        SDL_Log("SDL_BlitSurface failed: %s\n", SDL_GetError());
    }

    // Free the scaled surface
    SDL_FreeSurface(scaledButton);
}
