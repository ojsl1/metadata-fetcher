#include "main.h" // sdl
#include "render.h"
#include <sstream> // for fpsText

void RendererBase::initColors(AppContext gApp)
{
    gApp.pink = SDL_MapRGB(gApp.screen->format, 232, 111, 148);
    gApp.red = SDL_MapRGB(gApp.screen->format, 250, 0, 0);
    gApp.beige = SDL_MapRGB(gApp.screen->format, 255, 255, 115);
    gApp.blue = SDL_MapRGB(gApp.screen->format, 0, 0, 255);
    gApp.darkblue = SDL_MapRGB(gApp.screen->format, 111, 114, 120);
    gApp.darkgreen = SDL_MapRGB(gApp.screen->format, 100, 120, 100);
}

void RendererBase::initVideo( int window_width, int window_height )
{
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

    gApp.windowHandle = SDL_CreateWindow( "Metadata fetcher",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width, // window_width
                                window_height, // window_height
                                SDL_WINDOW_RESIZABLE
                                //SDL_WINDOW_FULLSCREEN_DESKTOP
                                );

    if ( gApp.windowHandle == nullptr ){
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return;
    }

    SDL_SetWindowBordered(gApp.windowHandle, SDL_TRUE);
    
    gApp.screen = SDL_GetWindowSurface(gApp.windowHandle);
    if ( gApp.screen == nullptr ){
      std::cerr << "SDL_ GetWindowSurface Error: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(gApp.windowHandle);
      SDL_Quit();
      return;
    }
}

void RendererBase::Clear()
{
  if (!gApp.screen){
    std::cout << "gApp.screen invalid during Clear, unable to clear." << std::endl;
  }
  SDL_FillRect(gApp.screen, nullptr, SDL_MapRGB(gApp.screen->format, 255, 50, 255));
}

void RendererBase::RenderMainMenu(Mouse &mouse, const MainMenuAssets &assets)
{
  assets.spriteBg.DrawScaled(gApp);
  assets.spriteBorder.DrawScaled(gApp);
  assets.spriteFrame.DrawScaled(gApp);
  assets.spriteDrop.DrawScaled(gApp);
  assets.spriteMute.Draw(gApp);
  assets.spritePause.Draw(gApp);
  assets.spriteTests.Draw(gApp);
  assets.spriteExit.Draw(gApp);

  assets.arial.Draw(gApp,80,200, "Drop Image Here", {0,0,0});
  assets.player.Draw(gApp);

  //Render droppedfile metadata
  int x = 50, y = 70;
  for (const std::string& line : gApp.pngInfo.lines){
    assets.arial.Draw(gApp, x, y, line, {0,0,0});
    y+= 25;
  }

  //Render FPS counter
  std::ostringstream fpsText;
  fpsText << "FPS: " << static_cast<float>(gApp.fps);
  assets.arial.Draw(gApp,10,10, fpsText.str(), {0,0,0});

  mouse.Draw(gApp);
}

void RendererBase::RenderMinigame(Mouse &mouse, const MinigameAssets &assets)
{
      assets.spritePlaceholder.DrawScaled(gApp);
      assets.spritePause.Draw(gApp);
      assets.player2.Draw(gApp);

      int x1 = 50;
      int y1 = 70;
      assets.arial.Draw(gApp,x1,y1, "RACE (1978)", {0,0,0});
      assets.arial.Draw(gApp,x1,y1+30, "UNIMPLEMENTED", {10,0,0});

  //Render FPS counter
  std::ostringstream fpsText;
  fpsText << "FPS: " << static_cast<float>(gApp.fps);
  assets.arial.Draw(gApp,10,10, fpsText.str(), {0,0,0});
  
  mouse.Draw(gApp);
}

void RendererBase::Update()
{
  SDL_UpdateWindowSurface(gApp.windowHandle);
}

void RendererBase::Shutdown(AppContext gApp)
{
    //SDL_SetWindowPosition( gApp.windowHandle, gApp.window.x, gApp.window.y ); //enforce position
    //SDL_SetWindowSize( gApp.windowHandle, gApp.window.w, gApp.window.h ); //enforce size
    SDL_GetWindowPosition( gApp.windowHandle, &gApp.window.x, &gApp.window.y );
    std::cout << "Exit Position: " << gApp.window.x << "," << gApp.window.y << std::endl;
    std::cout << "Exit Size: " << gApp.window.w << "," << gApp.window.h << " [FIXME: Doesnt update after resizing]" << std::endl;

    if (gApp.screen != nullptr) {
      SDL_FreeSurface(gApp.screen);
      gApp.screen = nullptr;
    }

    if (gApp.windowHandle != nullptr) {
      SDL_DestroyWindow(gApp.windowHandle);
      gApp.windowHandle = nullptr;
    }

    IMG_Quit();
    SDL_Quit();

    std::cout << "Exit succesfully" << std::endl;
}
