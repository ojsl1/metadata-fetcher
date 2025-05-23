#include "main.h" // globals
#include "render.h"
#include "character.h"

SDL_Window *gWindow = nullptr;
SDL_Surface *gScreen = nullptr;

void RendererBase::initColors(SDL_Surface *gScreen){
    gPink = SDL_MapRGB(gScreen->format, 232, 111, 148);
    gRed = SDL_MapRGB(gScreen->format, 250, 0, 0);
    gBeige = SDL_MapRGB(gScreen->format, 255, 255, 115);
    gBlue = SDL_MapRGB(gScreen->format, 0, 0, 255);
    gDarkblue = SDL_MapRGB(gScreen->format, 111, 114, 120);
    gDarkgreen = SDL_MapRGB(gScreen->format, 100, 120, 100);
}

void RendererBase::initVideo( int window_width, int window_height ){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
      std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
      return;
    }

    /*
    if (IMG_Init(IMG_INIT_PNG) != 0){
      std::cerr << "IMT_Init failed: " << IMG_GetError() << std::endl;
      return;
    }
    */

    if (TTF_Init() != 0){
      std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
      return;
    }

    // Create the window
    gWindow = SDL_CreateWindow( "Metadata fetcher",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width, // window_width
                                window_height, // window_height
                                SDL_WINDOW_RESIZABLE
                                //SDL_WINDOW_FULLSCREEN_DESKTOP
                                );

    if ( gWindow == nullptr ){
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return;
    }

    //Note: this disables SDL_WINDOW_FULLSCREEN_DESKTOP
    SDL_SetWindowBordered(gWindow, SDL_TRUE);
    
    gScreen = SDL_GetWindowSurface(gWindow);
    if ( gScreen == nullptr ){
      std::cerr << "SDL_ GetWindowSurface Error: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(gWindow);
      SDL_Quit();
      return;
    }
}

void RendererBase::Clear(){
  if (!gScreen){
    std::cout << "gScreen invalid during ren.Clear, unable to clear." << std::endl;
  }
  SDL_FillRect(gScreen, nullptr, SDL_MapRGB(gScreen->format, 255, 50, 255));
}

void RendererBase::Render(Mouse &mouse, Sprite &spriteExit, Sprite &spriteTests,
     Sprite &spriteDrop, Sprite &spriteMute, Sprite &spritePause,
     Sprite &spriteBorder, Sprite &spriteFrame, Sprite &spriteBg,
     Font &arial, Character &player){
  spriteBg.DrawScaled(gScreen);
  spriteBorder.DrawScaled(gScreen);
  spriteFrame.DrawScaled(gScreen);
  spriteDrop.DrawScaled(gScreen);
  spriteMute.Draw(gScreen);
  spritePause.Draw(gScreen);
  spriteExit.Draw(gScreen);

  //Render FPS counter
  std::ostringstream fpsText;
  fpsText << "FPS: " << static_cast<float>(fps);
  arial.Draw(gScreen,50,50, fpsText.str(), {0,0,0});

  arial.Draw(gScreen,80,200, "Drop Image Here", {0,0,0});

  #if ALLEYS
  spriteTests.Draw(gScreen);
  if (spriteTests.hasintersection){
    DrawTests();
  };
  #endif // ALLEYS
  
  player.Draw(gScreen);

  mouse.Draw(gScreen); // draw mouse last so it's always on top
}

void RendererBase::Update(){
  // Present the frame/update the new frame, same as SDL_RenderPresent()
  SDL_UpdateWindowSurface(gWindow);
}

void RendererBase::Shutdown(SDL_Window *gWindow, WindowDimensions dims){
    //SDL_SetWindowSize( gWindow, dims.wSize, dims.hSize ); //enforce size
    //SDL_SetWindowPosition( gWindow, dims.xPosi, dims.yPosi ); //enforce position
    SDL_GetWindowPosition( gWindow, &dims.xPosi, &dims.yPosi );
    std::cout << "Exit Position: " << dims.xPosi << "," << dims.yPosi << std::endl;
    std::cout << "Exit Size: " << dims.wSize << "," << dims.hSize << " [FIXME: Doesnt update after resizing]" << std::endl;

    if (gScreen != nullptr) {
      SDL_FreeSurface(gScreen);
      gScreen = nullptr;
    }

    if (gWindow != nullptr) {
      SDL_DestroyWindow(gWindow);
      gWindow = nullptr;
    }

    IMG_Quit();
    SDL_Quit();

    std::cout << "Exit succesfully" << std::endl;
}

#if ALLEYS
void RendererBase::DrawTests(){
  // ALLEY. Create SpriteTests
  SpriteTest object( gRed, WINDOW_WIDTH/2-20, WINDOW_HEIGHT/2-20 );
  SpriteTest another( gBlue, WINDOW_WIDTH/2-100, WINDOW_HEIGHT/2+20 );

  // ALLEY. Create Block spritetests
  Block block1( gPink, WINDOW_WIDTH/4, WINDOW_HEIGHT/8 );
  Block block2( gPink, WINDOW_WIDTH/2+60, WINDOW_HEIGHT/2-40 );
  block1.set_image( "assets/cosmox2.png" );
  block2.set_image( "assets/avatar2.webp" );

  /*
  TODO: ALLEY. Trying to blit cursor along with other spritetests
  Block block3mouse( blue, 130, 40 );
  block3mouse.set_image( "assets/mouse.png" );
  SpriteTestGroup pointers;
  pointers.add( &block3mouse );
  pointers.draw( gScreen ); // draw to mouse not to screen
  */

  // ALLEY. Create a SpriteTestGroup & add previously created Blocks to it
  SpriteTestGroup active_spritetests;
  active_spritetests.add( &block1 );
  active_spritetests.add( &block2 );
  active_spritetests.add( &another );
  active_spritetests.add( &object );
  object.draw( gScreen );

  // ALLEY. Test if SpriteTestGroup contains a specified spritetest_object
  //std::cout << active_spritetests.has( another ) << std::endl; // TODO: Print only once instead of spamming

  // ALLEY. Blit the whole SpriteTestGroup
  active_spritetests.draw(gScreen);
}
#endif // ALLEYS
