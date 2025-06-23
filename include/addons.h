#ifndef ADDONS_H
#define ADDONS_H

#include <vector>
#include <iostream>

void printTodos(){
  //(priority, description)
  // 1: now
  // 2: soon
  // 3: later
  // 4: maybe
  // 5: after project is done refactoring
  // 6: waste of time refactor ideas
  std::vector<std::pair<int, std::string>> todos = {

// GAME TODOS
{0, "BUG \n CANT MOVE (SOFTWARE) MOUSE WHILE MOVING MARISA!!"},
{0, "BUG \n deltaTime is not synchronized with the animation frames, leading to playAnimations looping too many times"},
{1, "BUG \n marisa: some sequences are of different w/h sizes than the idle animation, leading marisa to be offcenter"},

{0, "CLEANup \n now that colorkeying works move all the debug if-else to obsidian sdl notes"},
{1, "FEATURE \n what did i create the player.getX and player.getY for, collision detection?"},

{1, "REFACTOR \n sprite.h: move the sprite parameters from the constructor definitions into the draw call"
    "ie. from main.cpp to render.cpp inside the RendererBase::Render"},

{1, "OBSIDIAN \n BUG the rendered fps counter static_cast<int>(fps) shows just 0"
"A: the global fps definition was being overridden by the redundant local definition of fps on top of render.cpp, i didnt remove it because the global definition was giving linker errors, see below"
"A: the linker errors were coming from having defined `float fps = 0.0f;` inside the main functions scope and instead of outside of it ie. in the global scope"
"Lesson: practically the linker errors were happening because render.cpp was looking for the the global definitions from it's includes (main.h) and there were no fps, as apparently normal includes dont look inside functions like the main function"},
{1, "OBSIDIAN \n BUG marisa: to make marisa movable had to fix a circular-include-dependency"},


{1, "BUG \n cap_framerate() overshoots capping, ie. 144->167 400->420"},

{1, "rename all draw calls to render calls, see fe. render.cpp:Render()"},


 // METADATA TODOS
{1, "FEATURE \n render dragged image to gSreen"},

{1, "FEATURE \n render libpng metadata with Font:Render"},
{2, "FEATURE \n make the rendered metadata copyable to clipboard(s)"},


// WASTE OF TIME TODOS
{2, "FEATURE \n create a translucent box for fps a la simply love rendering stats"},

{2, "FEATURE \n resourcemanager.cpp"},

{2, "BUG PERF \n running inside gdb the app gets stuck in EventHandlerGlobal loop for at least 8-20 iters.."},

{5, "SDL3 \n https://github.com/libsdl-org/SDL/blob/main/docs/README-migration.md"},

// Unsorted (dump)
{1, "?? \n Abstract pure virtual class methods for runtime polymorphism"},

// Maybe important features
// GIMP EXPORT automatic pixel format: file <foo>.png "8-bit colormap" doesn't work.
// GIMP EXPORT setting pixel format to '8bpc RGBA': file <foo>.png "8-bit/color RGBA" works.
{2, "FEATURE \n automatic conversion of non-32bpp rectangles into 32bpp with SDL_ConvertSurface"},
{2, "FEATURE \n sprite.cpp: automatic conversion of non-32bpp rects into 32bpp inside SDL_BlitScaled"},

{2, "REFACTOR \n sprite.cpp: Draw and DrawScaled into overloads"},
{2, "REFACTOR \n sprite.cpp: make DrawScaled aspect-ratio lockable somehow or create a separate scaler function scaler"},
{2, "REFACTOR \n sprite.cpp: sprite.w and sprite.h values as percentages not pixels, 100percent being the raw px value"},


// Medium prio features
{3, "UI \n resize window and look at border thicknesses, they are wrong (note border uses DrawScaled now)"},
{3, "UI BUG \n resizing window resizes the DrawScaled images, are they dependant on ui reso? it also affects fps, doubling it after hiding all the sprites, plus breaking cap_framerate"},

{3, "DEBUG \n make the app not crash when assets dir is missing, and so it doesnt flood console with errors"},
{3, "DEBUG \n assets/texture-error.png relies on an asset when it shouldnt, maybe just fillrect with pink when the texture is missing"},
{3, "FEATURE \n parse comfyui metadata into readable format: the chunk keys are prompt and workflow"},


// Very low prio, dont even read these!
{5, "REFACTOR \n for performance separate DrawScaled() into Scale() and DrawScaled(), to avoid scaling every frame"},
{5, "REFACTOR \n instead of a compile time debug macro use an argv debug flag, afterwards combine the alleys and debug macros"},
{5, "REFACTOR \n font.h: fix the third overload or remove it"},

//Cleanup
{5, "DOCS \n font.cpp: is Font::Free abstraction from ~Font really necessary?"},
{4, "CLEANUP \n remove pygame classes from `render.h` into their own header files"},
{5, "DOCS \n input.h: document why the #pragma once is needed"},
{6, "DOCS \n sprite.cpp: document why SDL_BlitScaled and SDL_BlitSurface have different 3rd args"
    "ie. why does SDL_BlitScaled blit onto individual surfaces and SDL_BlitSurface directly onto gScreen"},

//Refactoring+Cleanup
{5, "REFACTOR \n input.h: refactor `point` as a private member with getters"},

// Below is garbage, waste of tim
  //Codestyle
{6, "OOP \n Element::Sprite::Mouse, Element::Sprite::Button,"},
{6, "OOP \n polymorphism: pick what virtual functions should made pure for Element"},
{6, "C++ \n classes with union keywords, for better memory management"},
{6, "C++ \n raii, sfinae, odr keywords, inline keywords"},
{6, "C++ \n is this project small enough for ODR and inline keywords or will it expand in the future"},
{4, "C++ \n github: pure c++ repo rename header files to .hpp?"},
{6, "C++ \n std::map<int, std::vector<std::string>> todos"},
{6, "C++11 \n range-based container iteration for below for loop"},
{6, "C++20 \n std::ranges for the below for loop"},
{5, "C++ \n Currently all header files that require SDL libs source them through 'render.h' -> 'main.h'."
    "Delete this superfluous step and directly source SDL libs through main.h."
    "For the affected header files this will require adding includes for the standard libs that are also in render.h"
    "(as of now just iostream and vector)."},
  //Just never
{6, "FEATURE \n Iterate over filenames image000...image991 and draw them one by one with some delay between"},
{6, "REFACTOR \n can `double delta` timer from main.cpp be combined with cap_framerate()"},
{6, "... \n For non-SDL_Mixer use SDL_PauseAudioDevice and SDL_OpenAudioDevice"},
{6, "... \n Win32"},
  //Renderering
{6, "... \n can you render mouse properly using the pygame-based SpriteTest pipeline"},
{6, "... \n RendererBase::renderer_sdl_sw, renderer_sdl_hw, renderer_opengl, renderer_vk"},
{6, "... \n renderer_opengl, vect2,"
"Replace all SDL_Rects with vertex buffers (VBO) and vertex array objects (VAO)"
  "If some SDL_Rects were texctured, load textures into opengl, bind them and adjust texture coords"
  "If some SDL_Rects were colored, pass color data into a separate buffer or through uniform variables within your shader"
"Simple vertex and fragment shaders: For positioning and transformations pass a matrix to the shader"
  "Pass a matrix to the shader for position and transformation operations (scaling/translating/rotating)"
"Reduce number of draw calls when calling lots of rects by batching them into a single vbo"
  "Dynamically update the buffer or use instancing for rendering multiple rects in a single call"},

};

  std::cout << "### Printing The Autosorted TODO List... ###" << std::endl;

  //print by priority
  //for (int priority = 1; priority <= 6; ++priority) {
  for (int priority = 6; priority >= 1; --priority) {
    for (const auto& todo : todos) {
      if (todo.first == priority) {
        std::cout << priority << ". " << todo.second << std::endl;
      }
    }
  }
}

#endif // ADDONS_H
