#ifndef MAIN_H
#define MAIN_H // MAIN.H "GLOBALS.H"

// Globals
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream> // for global debugging
#include <memory>
#include <GL/glew.h>

#define FPSCAP 60.0
#define DEBUG 0

enum class AppState
{
  MAIN_MENU,
  MINIGAME,
  EXIT,
};

struct WindowDimensions
{
    int w, h; // size
    int x, y; // position
};

void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );

using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
/**
 * @brief Runtime application context.
 * @var fps Current calculated fps.
 */
struct AppContext
{
    // runtime mode
    AppState mode = AppState::MAIN_MENU;

    // sdl handles (store pointers; ownership is elsewhere until using below smart pointers)
    //SDL_Renderer *renderer = nullptr; WIP for -sdl-gpu

    // TODO wrap both handles with smart pointers
    WindowPtr win{nullptr, SDL_DestroyWindow};
    SDL_Surface *screen = nullptr;

  //Shader loading utility programs
  void printProgramLog( GLuint program );
  void printShaderLog( GLuint shader );
  //Graphics program
  GLuint gProgramID = 0;
  GLint gVertexPos2DLocation = -1;
  GLuint gVBO = 0;
  GLuint gIBO = 0;

    // diagnostics
    float fps = 0.0f;
    WindowDimensions window;

    // palette
    Uint32 pink = 0, red = 0, beige = 0, blue = 0,
           darkblue = 0, darkgreen = 0;
};

extern AppContext gApp;

#endif // MAIN_H
