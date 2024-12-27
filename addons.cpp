#include "addons.h"
#include <vector>
#include <iostream>

void printVectorTodos(){
  //Store all to-dos in a vector of pairs (priority, description)
  // 1: current, 2: high, 3: medium, 4: low, 5: future
  std::vector<std::pair<int, std::string>> todos = {

{1, "GIMP \n CENTER FONT VERTICALLY ON BUTTONS"},
{1, "WIP button.cpp: implement an sdl_helper function inside SDL_BlitScaled to convert non-32bpp images to 32bpp"},
{1, "BACKLOG \n make the app runnable without the assets directory, and so it doesnt flood console"},
{1, "BACKLOG \n make the assets/texture-error.png not rely on an asset ie. just fillrect with pink tiles"},

{2, "WIP \n button.cpp: Finish `bool toggled` thus relying less on sdl"},
{2, "WIP \n input.cpp: Finish `bool clicked` thus relying less on sdl"},
{4, "WIP \n input.h: refactor `point` as a private member with getters"},
{4, "WIP \n button.cpp: turn into subclass of appobject"},

{3, "FEATURE \n button.cpp: implement a spritesheet"},
{3, "FEATURE \n button.cpp: change playButton sprite on interact using the commented spritesheet draw logic"},
{6, "FEATURE \n button.cpp: change playButton sprite on interact without a spritesheet"},
{4, "SDL2 \n button.cpp: SDL_BlitScaled and SDL_BlitSurface have different 3rd args\n ie. why does SDL_BlitScaled blit onto individual surfaces and SDL_BlitSurface directly onto gScreen"},

{4, "BACKLOG \n clicking X doesnt currently close the app"},
{4, "BACKLOG \n add listeners for ctrl+q and ctrl+w and q to exit the game"},

{6, "DOCS \n button.cpp: what is Button::DetectClicks for"},
{6, "DOCS \n input.h: document why the #pragma once is needed"},
{6, "BACKLOG \n is this project small enough for ODR and inline keywords or will it expand in the future"},
{6, "CLEANUP \n separate classes from `render.h` to their own header files"},
{6, "is it possible to draw mouse using the premade classes Sprite, SpriteGroup or Block"},
{6, "CLEANUP \n are window_width and window_height return types for initVideo necessary, considering its global/external"},
{6, "CLEANUP \n can `double delta` timer from main.cpp be combined with cap_framerate()"},

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
