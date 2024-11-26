#include "main.h" // globals
#include "render.h" // -> drawFrame()
#include "input.h" // -> render.h -> <vector>
#include "button.h"
#include "addons.h"
#include <iostream>

enum class MenuState {
    MAIN_MENU,
    GALLERY_MENU,
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
Button playButton(20, 230, 25, 25, "resources/button-inactive.png");
Button topleftButton(10, 10, 80, 80, "resources/play-pause.png");

// Color definitions
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

void loopMenuState(RendererBase& ren) {
  SDL_Event e;
  Mouse mouse; // cannot globally declare this because theres SDL_ShowCursor() inside that requires sdl to be initialized first
  while (SDL_PollEvent(&e)){
    switch (e.type){

      case SDL_MOUSEBUTTONUP:
        switch (e.button.button){
          case SDL_BUTTON_LEFT:{
              if (topleftButton.hasintersection){
                std::cout << "unimplemented" << std::endl;
              };
            }break;
        }

      case SDL_KEYDOWN:
        switch (e.key.keysym.sym){
          case SDLK_ESCAPE:{
            currentMenu = MenuState::EXIT;
          }break;
        }
      
      //WIP Music player interactions
      //TODO EVENT Clicked Play button... -> statePlayer = PLAYING;
      //TODO EVENT Render Play button into Pause button rect...
      //
      //TODO EVENT Clicked Pause button.. -> statePlayer = NOT_PLAYING
      //TODO EVENT Pause bgm music...
      //TODO EVENT Render Pause button into Play button rect...

    }
  }


  // Update cursor position
  mouse.Update();

  // Update the mouse state
  mouse.UpdateMouseState(e);
  // Use the mouse state for button toggle detection
  playButton.DetectClicks(mouse);

  // Detect button---mouse collisions
  playButton.DetectIntersections(mouse);
  topleftButton.DetectIntersections(mouse);

  ren.Clear();

  // Draw operations
  if (playButton.hasintersection){
    ren.DrawAlleys();
  };
  ren.DrawMainMenu();
  playButton.Draw(gScreen);
  topleftButton.DrawScaled(gScreen);
  mouse.Draw(gScreen); // draw mouse last so it's always on top
  
  ren.Present();
}

void loopGalleryState() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "App entered unimplemented state, exiting." << std::endl;
    currentMenu = MenuState::EXIT;
    //add gallery-specific input handling
    //TODO EVENT Returning to Main Menu... -> currentMenu = MAIN_MENU;
  }
  // Render gallery state (ie. gallery viewport, images etc.)
}

void renderMenus(RendererBase& ren){
    switch (currentMenu) {
      case MenuState::MAIN_MENU:
          loopMenuState(ren);
          break;
      case MenuState::GALLERY_MENU:
          loopGalleryState();
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
  ren.initVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
  ren.initColors(gScreen);
  ren.initMixer();

  Uint32 starting_tick;

  currentMenu = MenuState::MAIN_MENU;
  Mouse mouse; // cant have this globally declared because it has SDL stuff inside that need to be initialized first
  
  while (true){
    starting_tick = SDL_GetTicks();
    ren.cap_framerate( starting_tick );

    /*
    // Handle input to switch between menus
    handleMenuInput();
    */

    // Render the appropriate menu based on the currentMenu state.
    renderMenus(ren);

    if (currentMenu == MenuState::EXIT){
        break;
    }
  }
  
  ren.Shutdown(gWindow, bell, bgm, dims);
  return 0;
}

