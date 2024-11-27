#include "addons.h"
#include <vector>
#include <iostream>

void printVectorTodos(){
  //Store all to-dos in a vector of pairs (priority, description)
  // 1: current, 2: high, 3: medium, 4: low, 5: future
  std::vector<std::pair<int, std::string>> todos = {

    {1, "FEATURE \n is this project small enough for ODR and inline keywords or will it expand in the future"},
    {1, "CLEANUP \n Document why input.h needs the #pragma once."},
    {1, "WIP \n Finish turning all classes into subclasses of appobject, button not done, mouse done"},
    {1, "WIP \n Finish implementing the button.cpp `bool toggled` and input.cpp `bool clicked` for centralizing input handling, using less sdl and cleaner code overall"},
    {1, "REFACTOR \n play_state into a separate player subclass, that has the playing/notplaying/paused etc states, and maybe even the play/pause button rects"},
    {1, "FEATURE \n handleMenuState(): move the draw operations for buttons etc to ren.draw"},
    {1, "FEATURE \n renderer.cpp create separate draw functions for different menus: drawMainMenu, drawPlayer, drawGallery, etc"},

    {1, "REFACTOR \n button.cpp: add rect dimensions into the class arguments, to avoid having to resize image files"},
    {1, "SDL2 THEORY \n button.cpp: why are the 3rd args of SDL_BlitScaled() and SDL_BlitSurface() different? the SDL_BlitScaled was yoinked from input.cpp scaling logic"},

    {1, "FEATURE \n drawButtonScaled(): the 2nd & 3rd dRectButton args dont scale properly, aspect ratio shenanigans?"},
    {1, "REFACTOR \n input.h: refactor point as a private member with getters"},
    {1, "CLEANUP \n to avoid confusion make sure all variables are declared only either GLOBALLY or LOCALLY"},


    {2, "BACKLOG \n Figure out the relationship between all the 50,50 values"},

    {3, "BACKLOG \n main.cpp: if play rect pressed -> currentState = PLAY_STATE"},
    {3, "BACKLOG \n main.cpp: if gallery rect pressed -> currentState = GAY_STATE"},
    {3, "BACKLOG \n Clicking X to close the game"},
    {3, "BACKLOG \n button.cpp: change playButton sprite on interact using the commented spritesheet drawing logic"},
    {6, "DEPENDS button.cpp: change playButton sprite on interact without a spritesheet"},
    {6, "Rewrite the eventloop as a separate EventListener (mouse events) -> clickable buttons -> main menu -> submenus"},

    {6, "Separate classes from `render.h` to their own header files"},
    {6, "QUESTION: possible to draw mouse using the premade classes Sprite, SpriteGroup or Block"},
    {6, "was adding window_width and window_height return types to initVideo() really necessary when you made it a global/extern function"},

    {4, "Add listeners for ctrl+q and ctrl+w and q to exit the game"},

    {6, "FEATURE \n test if `double delta` timer from main.cpp can be combined with cap_framerate()"},
    {5, "FEATURE \n button.cpp: implement spritesheet capabilities"},
  };

  std::cout << "### Printing The Autosorted TODO List... ###" << std::endl;

  //print todos by priority
  //for (int priority = 1; priority <= 6; ++priority) {
  for (int priority = 6; priority >= 1; --priority) {
    for (const auto& todo : todos) {
      if (todo.first == priority) {
        std::cout << priority << ". " << todo.second << std::endl;
      }
    }
  }
}
