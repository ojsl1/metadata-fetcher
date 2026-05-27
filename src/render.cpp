#include "main.h" // sdl
#include "render.h"
#include "sprite.h"
#include "util.h"
#include <sstream> // for fpsText
#include <iostream>

void SurfaceRenderer::setColors(AppContext &ctx)
{
    ctx.pink = SDL_MapRGB(ctx.screen->format, 232, 111, 148);
    ctx.red = SDL_MapRGB(ctx.screen->format, 250, 0, 0);
    ctx.beige = SDL_MapRGB(ctx.screen->format, 255, 255, 115);
    ctx.blue = SDL_MapRGB(ctx.screen->format, 0, 0, 255);
    ctx.darkblue = SDL_MapRGB(ctx.screen->format, 111, 114, 120);
    ctx.darkgreen = SDL_MapRGB(ctx.screen->format, 100, 120, 100);
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

void SurfaceRenderer::Draw(const SurfaceSprite &sprite, AppContext ctx){
  SDL_Surface *currentSprite =
    sprite.toggled && sprite.alternateSprite
    ? sprite.alternateSprite
    : sprite.rawSprite;

  if (!currentSprite || !ctx.screen)
    return;

  // const SDL_Rect is not allowed, create local:
  SDL_Rect dstRect = sprite.dRectSprite;

  SDL_BlitSurface(currentSprite, nullptr, ctx.screen, &dstRect);
}

void SurfaceRenderer::DrawScaled(const SurfaceSprite &sprite, AppContext ctx){
  if (!sprite.rawSprite || !ctx.screen){
      SDL_Log("Invalid input surface (rawSprite or screen is null)\n");
      return;
  }

  #if DEBUG
  // Log and check rawSprite bpp.
  SDL_Log("rawSprite BitsPerPixel: %d\n", rawSprite->format->BitsPerPixel);
  if (rawSprite->format->BitsPerPixel != 32 &&
      rawSprite->format->BitsPerPixel != 16 &&
      rawSprite->format->BitsPerPixel != 8) {
      SDL_Log("Unsupported rawSprite format: %d bpp\n", rawSprite->format->BitsPerPixel);
      return;
  }
  // Log rawSprite pixel format details.
  SDL_Log("rawSprite Pixel Format: Rmask: 0x%X, Gmask: 0x%X, Bmask: 0x%X, Amask: 0x%X\n",
          rawSprite->format->Rmask,
          rawSprite->format->Gmask,
          rawSprite->format->Bmask,
          rawSprite->format->Amask);
  #endif

  // Create a 32bpp scaled surface
  SDL_Surface *scaledSpriteLocal = SDL_CreateRGBSurface(
      sprite.rawSprite->flags,
      sprite.dRectSprite.w,
      sprite.dRectSprite.h,
      32,
      sprite.rawSprite->format->Rmask,
      sprite.rawSprite->format->Gmask,
      sprite.rawSprite->format->Bmask,
      sprite.rawSprite->format->Amask);

  if (!scaledSpriteLocal){
    SDL_Log("SDL_CreateRGBSurface failed: %s\n", SDL_GetError()); // e.g. "Unknown pixel format"
    return;
  }

  /**
   * @brief Create a local destination rectangle for custom scaling
   * @param Replace the fourth arg of SDL_BlitScaled with &scaleRect
   * @details If you want to scale only a portion of the rawSprite or
   * scale it to a specific region of the destination surface, e.g. if
   * scaleRect.w and scaleRect.h differ from scaledSprite.
   * @notes If the entire rawSprite is scaled to the exact dimensions of
   * scaledSpriteLocal, you don't need scaleRect. SDL_BlitScaled automatically matches
   * the size of the source surface to the destination surface if NULL is passed.
   */
   //SDL_Rect scaleRect = {0, 0, dRectSprite.w, dRectSprite.h}; // x,y,w,h

  // Scale rawSprite on scaledSpriteLocal.
  if (SDL_BlitScaled(sprite.rawSprite, nullptr, scaledSpriteLocal, nullptr) < 0){
      // NB: scaledSpriteLocal is 32bpp, the images have to also be 32bpp.
      SDL_Log("SDL_BlitScaled failed: %s\n", SDL_GetError()); // e.g. "Blit combination not supported"
      SDL_FreeSurface(scaledSpriteLocal);
      return;
  }

  // const SDL_Rect is not allowed, create local:
  SDL_Rect dstRect = sprite.dRectSprite;

  // Blit scaledSpriteLocal onto the screen.
  if (SDL_BlitSurface(scaledSpriteLocal, nullptr, ctx.screen, &dstRect) < 0){
      std::cout << "rawSprite BitsPerPixel: " << sprite.rawSprite->format->BitsPerPixel << std::endl;
      std::cout << "scaledSprite BitsPerPixel: " << scaledSpriteLocal->format->BitsPerPixel << std::endl;
      SDL_Log("SDL_BlitSurface failed: %s\n", SDL_GetError());
  }

  // Free the local surface
  SDL_FreeSurface(scaledSpriteLocal);
}

void SurfaceRenderer::RenderMainMenu(Mouse &mouse, const MainMenuAssets &assets)
{
  DrawScaled(*assets.spriteBg, gApp);
  DrawScaled(*assets.spriteBorder, gApp);
  DrawScaled(*assets.spriteFrame, gApp);
  DrawScaled(*assets.spriteDrop, gApp);
  Draw(*assets.spriteMute, gApp);
  Draw(*assets.spritePause, gApp);
  Draw(*assets.spriteTests, gApp);
  Draw(*assets.spriteExit, gApp);

  assets.arial->Draw(gApp,80,200, "Drop Image Here", {0,0,0});
  assets.player->DrawPlayer(gApp);

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
  DrawScaled(*assets.spritePlaceholder, gApp);
  DrawScaled(*assets.spritePause, gApp);
  assets.player2->DrawPlayer(gApp);

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

void SurfaceRenderer::Shutdown(AppContext &ctx)
{
    //SDL_SetWindowPosition( gApp.windowHandle, gApp.window.x, gApp.window.y ); //enforce position
    //SDL_SetWindowSize( gApp.windowHandle, gApp.window.w, gApp.window.h ); //enforce size
    SDL_GetWindowPosition( ctx.win, &ctx.window.x, &ctx.window.y );
    std::cout << "Exit Position: " << ctx.window.x << "," << ctx.window.y << std::endl;
    std::cout << "Exit Size: " << ctx.window.w << "," << ctx.window.h << " [FIXME: Doesnt update after resizing]" << std::endl;

    if (ctx.screen != nullptr) {
      SDL_FreeSurface(ctx.screen);
      ctx.screen = nullptr;
    }

    if (ctx.win != nullptr) {
      SDL_DestroyWindow(ctx.win);
      ctx.win = nullptr;
    }

    IMG_Quit();
    SDL_Quit();

    std::cout << "Exit successfully" << std::endl;
}
