#include "addons.h"
#include <vector>
#include <iostream>

void printVectorTodos(){
  //Store all to-dos in a vector of pairs (priority, description)
  // 1: now
  // 2: soon
  // 3: later
  // 4: maybe
  // 5: after project is done refactoring
  // 6: waste of time refactor ideas
  std::vector<std::pair<int, std::string>> todos = {

{1, "GIMP \n transparency for dragndrop image"},
{1, "BUG \n button.cpp: button.w, butotn.h are broken for foo.Draw sprites as only DrawScaled uses them"},
{1, "CLEANUP \n github: pure c++ repo"},

{2, "COMPATIBILITY \n SDL_ConvertSurface non-32bpp rectangles"},
{2, "FEATURE \n SDL_Text to render text"},  
{2, "FEATURE \n button.cpp: spritesheet"},

{3, "CLEANUP \n appobject.h: subclasses must implement all the pure virtual functions."},

{3, "FEATURE \n button.cpp: toggle button-pause sprite using the commented spritesheet draw logic"},
{4, "FEATURE \n button.cpp: toggle button-pause sprite without a spritesheet"},

{3, "WIP button.cpp: implement an sdl_helper function inside SDL_BlitScaled to convert non-32bpp images to 32bpp"},
{3, "COMPATIBILITY \n make the app runnable without the assets directory, and so it doesnt flood console"},
{3, "COMPATIBILITY \n make the assets/texture-error.png not rely on an asset ie. just fillrect with pink tiles"},

{4, "WIP \n input.h: refactor `point` as a private member with getters"},
{4, "WIP \n button.cpp: turn into subclass of appobject"},

{4, "DOCS \n button.cpp: SDL_BlitScaled and SDL_BlitSurface have different 3rd args\n ie. why does SDL_BlitScaled blit onto individual surfaces and SDL_BlitSurface directly onto gScreen"},

{5, "DOCS \n input.h: document why the #pragma once is needed"},
{5, "CLEANUP \n separate pygame classes from `render.h` to their own header files"},
{5, "can you render mouse properly using the pygame based test classes Sprite/SpriteGroup/Block"},
{5, "CLEANUP \n are window_width and window_height return types for initVideo necessary, considering its global/external"},
{5, "CLEANUP \n can `double delta` timer from main.cpp be combined with cap_framerate()"},
{5, "PERF \n separate DrawScaled() into Scale() and DrawScaled(), to avoid scaling every frame"},
{5, "FEATURE \n instead of a compile time debug macro use an argv debug flag, afterwards combine the alleys and debug macros"},

{6, "BACKLOG \n For non-SDL_Mixer use SDL_PauseAudioDevice and SDL_OpenAudioDevice"},
{6, "BACKLOG \n is this project small enough for ODR and inline keywords or will it expand in the future"},
{6, "BACKLOG \n std::map<int, std::vector<std::string>> todos"},
{6, "BACKLOG \n C++11 range-based container iteration for below for loop"},
{6, "BACKLOG \n C++20 std::ranges for the below for loop"},

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
