#include "main.h" // sdl
#include "render.h"
#include "util.h"
#include <sstream> // for fpsText
#include <iostream>

void SurfaceRenderer::setColors(AppContext &gApp)
{
    gApp.pink = SDL_MapRGB(gApp.screen->format, 232, 111, 148);
    gApp.red = SDL_MapRGB(gApp.screen->format, 250, 0, 0);
    gApp.beige = SDL_MapRGB(gApp.screen->format, 255, 255, 115);
    gApp.blue = SDL_MapRGB(gApp.screen->format, 0, 0, 255);
    gApp.darkblue = SDL_MapRGB(gApp.screen->format, 111, 114, 120);
    gApp.darkgreen = SDL_MapRGB(gApp.screen->format, 100, 120, 100);
}

void SurfaceRenderer::initSubsystems( int window_width, int window_height )
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0){
      std::cerr << "Video disabled: " << SDL_GetError() << std::endl;
      return;
    }

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0){
      std::cerr << "Audio disabled: " << SDL_GetError() << std::endl;
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

    gApp.win = SDL_CreateWindow( "Metadata fetcher",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width, // window_width
                                window_height, // window_height
                                SDL_WINDOW_RESIZABLE
                                //SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    if ( gApp.win == nullptr ){
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return;
    }

    SDL_SetWindowBordered(gApp.win, SDL_TRUE);

    /*
    // TODO: Co-existence of a SDL_CreateRenderer and SDL_GetWindowSurface
    gApp.renderer = SDL_CreateRenderer(gApp.win, -1, 0);

    if (SDL_GetRenderer(gApp.win) == 0){
      std::cerr << "Creating any renderer failed: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return;
    }
    */

//    if (gApp.render_opengl = true){
//        // OPENGL RENDER
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        SDL_GL_SwapWindow(gApp.windowHandle);
//    }else{
//        // SOFTWARE RENDER
//    }

    gApp.screen = SDL_GetWindowSurface(gApp.win);
    if ( gApp.screen == nullptr ){
      std::cerr << "SDL_GetWindowSurface Error: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(gApp.win);
      SDL_Quit();
      return;
    }
}

void SurfaceRenderer::Clear()
{
  if (!gApp.screen){
    std::cout << "gApp.screen invalid during Clear, unable to clear." << std::endl;
  }
  SDL_FillRect(gApp.screen, nullptr, SDL_MapRGB(gApp.screen->format, 255, 50, 255));
}

void SurfaceRenderer::RenderMainMenu(Mouse &mouse, const MainMenuAssets &assets)
{
  assets.spriteBg->DrawScaled(gApp);
  assets.spriteBorder->DrawScaled(gApp);
  assets.spriteFrame->DrawScaled(gApp);
  assets.spriteDrop->DrawScaled(gApp);
  assets.spriteMute->Draw(gApp);
  assets.spritePause->Draw(gApp);
  assets.spriteTests->Draw(gApp);
  assets.spriteExit->Draw(gApp);

  assets.arial->Draw(gApp,80,200, "Drop Image Here", {0,0,0});
  assets.player->Draw(gApp);

  //Render droppedfile metadata
  int x = 50, y = 70;
  for (const std::string& line : pngInfo.lines){
    assets.arial->Draw(gApp, x, y, line, {0,0,0});
    y+= 25;
  }

  //Render FPS counter
  std::ostringstream fpsText;
  fpsText << "FPS: " << static_cast<float>(gApp.fps);
  assets.arial->Draw(gApp,10,10, fpsText.str(), {0,0,0});

  mouse.Draw(gApp);
}

void SurfaceRenderer::RenderMinigame(Mouse &mouse, const MinigameAssets &assets)
{
  assets.spritePlaceholder->DrawScaled(gApp);
  assets.spritePause->Draw(gApp);
  assets.player2->Draw(gApp);

  int x1 = 50;
  int y1 = 70;
  assets.arial->Draw(gApp,x1,y1, "RACE (1978)", {0,0,0});
  assets.arial->Draw(gApp,x1,y1+30, "UNIMPLEMENTED", {10,0,0});

  mouse.Draw(gApp);
}

void SurfaceRenderer::Update()
{
  SDL_UpdateWindowSurface(gApp.win);
}

void SurfaceRenderer::Shutdown(AppContext &gApp)
{
    //SDL_SetWindowPosition( gApp.windowHandle, gApp.window.x, gApp.window.y ); //enforce position
    //SDL_SetWindowSize( gApp.windowHandle, gApp.window.w, gApp.window.h ); //enforce size
    SDL_GetWindowPosition( gApp.win, &gApp.window.x, &gApp.window.y );
    std::cout << "Exit Position: " << gApp.window.x << "," << gApp.window.y << std::endl;
    std::cout << "Exit Size: " << gApp.window.w << "," << gApp.window.h << " [FIXME: Doesnt update after resizing]" << std::endl;

    if (gApp.screen != nullptr) {
      SDL_FreeSurface(gApp.screen);
      gApp.screen = nullptr;
    }

    if (gApp.win != nullptr) {
      SDL_DestroyWindow(gApp.win);
      gApp.win = nullptr;
    }

    IMG_Quit();
    SDL_Quit();

    std::cout << "Exit succesfully" << std::endl;
}
