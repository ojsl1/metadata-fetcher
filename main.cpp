#include "main.h" // globals
#include "render.h" // -> drawFrame()
#include "input.h" // -> render.h -> <vector>
#include "button.h"
#include "addons.h"
#include "audio.h"
#include <iostream>

enum class MenuState {
    MAIN_MENU,
    EXIT,
};

// System definitions
MenuState currentMenu = MenuState::MAIN_MENU;
double delta;

// Debug definitions
WindowDimensions dims;

// Resource definitions
Mix_Chunk *bell;
Mix_Music *bgm;
Button buttonAlleys(10, 220, 25, 25, "assets/button-alleysshow.png");
Button buttonExit(10, 270, 80, 80, "assets/button-exit.png");

// Color definitions
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

// Detect all button intersections
void DetectButtonIntersections(Mouse& mouse){
  buttonAlleys.DetectIntersections(mouse);
  buttonExit.DetectIntersections(mouse);
}

void UpdateMouseInteractions(Mouse& mouse, SDL_Event& e){
  mouse.Update();
  mouse.UpdateMouseState(e);// TODO IS THIS FUNCTION BROKEN, WTF IS IT FOR?
  // TODO mouse.clicked is broken - Use the mouse state for button toggle detection
  buttonAlleys.DetectClicks(mouse);
  DetectButtonIntersections(mouse);
}

void handleMainMenuState(RendererBase& ren, Mouse& mouse, const SDL_Event& e) {
  switch (e.type){
    case SDL_MOUSEBUTTONUP:
      switch (e.button.button){
        case SDL_BUTTON_LEFT:{
          if (buttonExit.hasintersection){
          currentMenu = MenuState::EXIT;
          };
        }break;
      }

    case SDL_KEYDOWN:
      switch (e.key.keysym.sym){
        case SDLK_ESCAPE:{
          currentMenu = MenuState::EXIT;
        }break;
      }
  } 
}

void renderMainMenuState(RendererBase& ren, Mouse& mouse, SDL_Event& e){
  UpdateMouseInteractions(mouse, e);
  ren.Draw(mouse,buttonExit,buttonAlleys);
  ren.Present();
}

void updateState(RendererBase& ren, Mouse& mouse, SDL_Event& e){
  switch (currentMenu) {
    case MenuState::MAIN_MENU:
        handleMainMenuState(ren,mouse,e);
        break;
    case MenuState::EXIT:
        std::cout << "[TODO: this doesnt get printed out]" << std::endl;
        break;
    default:
        break;
  }
}


void renderState(RendererBase& ren, Mouse& mouse, SDL_Event& e){
  switch (currentMenu) {
    case MenuState::MAIN_MENU:
        renderMainMenuState(ren,mouse,e);
        break;
    case MenuState::EXIT:
        std::cout << "[TODO: this doesnt get printed out]" << std::endl;
        break;
    default:
        break;
  }
}

int main (int argc, char *argv[]){
  printVectorTodos();

  RendererBase ren;
  Audio audio;

  ren.initVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
  ren.initColors(gScreen);
  audio.initMixer();

  Uint32 starting_tick;
  currentMenu = MenuState::MAIN_MENU;

  Mouse mouse; // cant have this globally declared because it has SDL stuff inside that need to be initialized first
  
  while (currentMenu != MenuState::EXIT){
    starting_tick = SDL_GetTicks();

    SDL_Event e;
    while (SDL_PollEvent(&e)){
        updateState(ren,mouse,e); // handle events for the current state
    }

    renderState(ren,mouse,e); // render the current state
    ren.cap_framerate(starting_tick);
  }
 
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gWindow, dims);
  return 0;
}

