#ifndef MAIN_H
#define MAIN_H // MAIN.H "GLOBALS.H"

// Globals
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <iostream>

// Locals
#include <vector> // struct PNGDisplayInfo
#include <string> // struct PNGDisplayInfo

#define FPSCAP 60
#define DEBUG 0

enum class AppState
{
  MAIN_MENU,
  MINIGAME,
  EXIT,
};

struct PNGDisplayInfo
{
  uint32_t height;
  uint32_t width;
  int bitDepth;
  std::vector<std::string> lines;
};

struct WindowDimensions
{
    int w, h; // size
    int x, y; // position
};

struct AppContext
{
    // runtime mode
    AppState mode = AppState::MAIN_MENU;

    // last-dropped png metadata 
    PNGDisplayInfo pngInfo;

    // sdl handles (store pointers; ownership is elsewhere until using below smart pointers)
    SDL_Window *windowHandle = nullptr;
    SDL_Surface *screen = nullptr;

    // TODO wrap both handles with smart pointers
    //before struct: `using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;`
    // -> WindowPtr windowHandle(nullptr, SDL_DestroyWindow);
    //before struct: `using SurfacePtr = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface>;`
    // -> SurfacePtr screen(nullptr, SDL_FreeSurface);

    // diagnostics
    float fps = 0.0f;
    WindowDimensions window;

    // palette
    Uint32 pink = 0, red = 0, beige = 0, blue = 0,
           darkblue = 0, darkgreen = 0;
};

extern AppContext gApp;

#endif // MAIN_H
