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
Button buttonShowAlleys(75, 150, 25, 25, "assets/button-alleysshow.png");
Button buttonExit(75, 230, 80, 80, "assets/button-exit.png");

// Color definitions
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

// Detect all button intersections
void DetectButtonIntersections(Mouse& mouse){
  buttonShowAlleys.DetectIntersections(mouse);
  buttonExit.DetectIntersections(mouse);
}

void UpdateMouseInteractions(Mouse& mouse, SDL_Event& e){
  mouse.Update();
  mouse.UpdateMouseState(e);
  // Use the mouse state for button toggle detection
  buttonShowAlleys.DetectClicks(mouse);
  DetectButtonIntersections(mouse);
}

//TODO This method should be moved under RendererBase in the future once you've settled on the menus
void DrawUI(RendererBase& ren, Mouse& mouse){
  ren.Clear();

  #if ALLEYS
  if (buttonShowAlleys.hasintersection){
    ren.DrawAlleys();
  };
  buttonShowAlleys.Draw(gScreen);
  #endif // ALLEYS
  buttonExit.Draw(gScreen);

  //buttonExit.DrawScaled(gScreen); // TODO this doesnt work?
  mouse.Draw(gScreen); // draw mouse last so it's always on top
}

void loopMenuState(RendererBase& ren) {
  SDL_Event e;
  Mouse mouse; // cannot globally declare this because theres SDL_ShowCursor() inside that requires sdl to be initialized first
  while (SDL_PollEvent(&e)){
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

  UpdateMouseInteractions(mouse, e);
  DrawUI(ren, mouse);
  ren.Present();
}

void renderMenus(RendererBase& ren){
    switch (currentMenu) {
      case MenuState::MAIN_MENU:
          loopMenuState(ren);
          break;
      case MenuState::EXIT:
          std::cout << "[TODO: this doesnt get printed out]" << std::endl;
          break;
      default:
          std::cout << "This shouldn't happen" << std::endl;
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
  
  while (true){
    starting_tick = SDL_GetTicks();
    ren.cap_framerate( starting_tick );

    // Render the appropriate menu based on the currentMenu state.
    renderMenus(ren);

    if (currentMenu == MenuState::EXIT){
        break;
    }

    // end of main loop
  }
 
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gWindow, dims);
  return 0;
}

