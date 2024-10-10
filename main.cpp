#include "main.h" // globals
#include "render.h" // -> drawFrame()
#include "input.h" // -> render.h -> <vector>
#include "button.h"
#include "addons.h"

enum class MenuState {
    MAIN_MENU,
    PLAY_MENU,
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

void loopMenuState(Renderer& ren) {
  SDL_Event e;
  Mouse mouse; // cannot globally declare this because theres SDL_ShowCursor() inside that requires sdl to be initialized first
  while (SDL_PollEvent(&e)) {
    switch (e.type){

      case SDL_MOUSEBUTTONUP:
        switch (e.button.button){

          case SDL_BUTTON_LEFT:{
            std::cout << "value is: " <<  playButton.selected << std::endl;
            if (playButton.selected){std::cout << "Play Music... (m1)" << std::endl;};
            if (topleftButton.selected){currentMenu = MenuState::PLAY_MENU;}
            } break;

        } break;

      case SDL_KEYDOWN:{
          switch (e.key.keysym.sym){
            case SDLK_ESCAPE:{currentMenu = MenuState::EXIT;} break;
          }
      }break;
      
      //WIP Music player interactions
      //TODO EVENT Clicked Play button... -> statePlayer = PLAYING;
      //TODO EVENT Render Play button into Pause button rect...
      //
      //TODO EVENT Clicked Pause button.. -> statePlayer = NOT_PLAYING
      //TODO EVENT Pause bgm music...
      //TODO EVENT Render Pause button into Play button rect...

      default: break;} //end - switch (e.type){
  }//end - while (SDL_PollEvent(&e)){


  // Update cursor position
  mouse.Update();

  // Detect button---cursor collisions
  playButton.DetectCollisions(mouse);
  topleftButton.DetectCollisions(mouse);

  ren.clearScreen();
  ren.drawMainMenu();

  // Draw the buttons
  playButton.Draw(gScreen);
  topleftButton.DrawScaled(gScreen);

  // Draw the cursor last ie. on top of everything else
  mouse.Draw(gScreen);
  
  // Update the window surface ie. display new draw ops ie. new frame
  SDL_UpdateWindowSurface(gWindow);

}

void loopPlayState() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "App entered deprecated state, exiting." << std::endl;
    currentMenu = MenuState::EXIT;
  }
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

void renderMenus(Renderer& ren){
    switch (currentMenu) {
      case MenuState::MAIN_MENU:
          loopMenuState(ren);
          break;
      case MenuState::PLAY_MENU:
          loopPlayState();
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

  Renderer ren;
  ren.initVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
  ren.initColors(gScreen);
  ren.initMixer();

  Timer timer;
  timer.Start();

  Uint32 starting_tick;

  currentMenu = MenuState::MAIN_MENU;
  Mouse mouse; // cant have this globally declared because it has SDL stuff inside that need to be initialized first
  
  while (true){
    starting_tick = SDL_GetTicks();
    ren.cap_framerate( starting_tick );

    timer.Update();
    float deltaTime = timer.getDeltaTime();
    //std::cout << "Delta time: " << deltaTime << " seconds" << std::endl;

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
  
  ren.cleanup(gWindow, bell, bgm, dims);
  return 0;
}

