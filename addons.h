#ifndef ADDONS_H
#define ADDONS_H
#pragma once

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

{1, "BUG \n resizing window resizes the DrawScaled images, are they dependant on ui reso?"},
{1, "GUI \n resize window and look at border thicknesses, they are wrong..."},
{2, "BUGS \n font.h:fix the overlaods"},
{2, "BUGS \n sprite.h: move the sprite parameters from the constructor into the draw call"
    "ie. from main.cpp to render.cpp inside the RendererBase::Draw"},

{2, "BUGS \n fix compiler Wreorder warnings"},
{2, "FEATURE \n SDL_Text render FPS every frame"},  
{3, "GITHUB \n pure c++ repo, see newest 'Style' commit, before that C was 1.3%"},

// GIMP EXPORT automatic pixel format: file <foo>.png "8-bit colormap" doesn't work.
// GIMP EXPORT setting pixel format to '8bpc RGBA': file <foo>.png "8-bit/color RGBA" works.
{2, "COMPATIBILITY \n SDL_ConvertSurface non-32bpp rectangles into 32bpp"},

{2, "REFACTOR \n SDL_Renderer for hardware acceleration, afterwards checkRendererType(SDL_Renderer *ren)"},

{1, "CLEANUP \n sprite.cpp: aspect ratio-locked scaler to constructor, instead of the freesize w,h variables"},
{2, "CLEANUP \n sprite.cpp: sprite.w and sprite.h values as percentages not pixels, 100percent being the raw px value"},
{3, "CLEANUP \n sprite.cpp using sprite.w and sprite.h with calculating alternateSprite location?"},

{3, "CLEANUP \n element.h: subclasses must implement all the pure virtual functions."},


{3, "WIP sprite.cpp: implement an sdl_helper function inside SDL_BlitScaled to convert non-32bpp images to 32bpp"},
{3, "COMPATIBILITY \n make the app runnable without the assets directory, and so it doesnt flood console"},
{3, "COMPATIBILITY \n make the assets/texture-error.png not rely on an asset ie. just fillrect with pink tiles"},

{4, "WIP \n input.h: refactor `point` as a private member with getters"},
{4, "WIP \n sprite.cpp: turn into subclass of element"},

{4, "DOCS \n sprite.cpp: SDL_BlitScaled and SDL_BlitSurface have different 3rd args\n ie. why does SDL_BlitScaled blit onto individual surfaces and SDL_BlitSurface directly onto gScreen"},
{4, "FEATURE \n toggle spritePause without a spritesheet"},

{5, "DOCS \n input.h: document why the #pragma once is needed"},
{5, "CLEANUP \n separate pygame classes from `render.h` to their own header files"},
{5, "can you render mouse properly using the pygame-based SpriteTest pipeline"},
{5, "CLEANUP \n can `double delta` timer from main.cpp be combined with cap_framerate()"},
{5, "PERF \n separate DrawScaled() into Scale() and DrawScaled(), to avoid scaling every frame"},
{5, "FEATURE \n instead of a compile time debug macro use an argv debug flag, afterwards combine the alleys and debug macros"},

{6, "BACKLOG \n For non-SDL_Mixer use SDL_PauseAudioDevice and SDL_OpenAudioDevice"},
{6, "BACKLOG \n is this project small enough for ODR and inline keywords or will it expand in the future"},
{6, "BACKLOG \n std::map<int, std::vector<std::string>> todos"},
{6, "BACKLOG \n C++11 range-based container iteration for below for loop"},
{6, "BACKLOG \n C++20 std::ranges for the below for loop"},

{6, "CLEANUP \n Currently all header files that require SDL libs source them through 'render.h' -> 'main.h'."
    "Delete this superfluous step and directly source SDL libs through main.h."
    "For the affected header files this will require adding includes for the standard libs that are also in render.h"
    "(as of now just iostream and vector)."},

{6, "BACKLOG \n Element::Sprite::Mouse, Element::Sprite::Button"},
{6, "BACKLOG \n class polymorphism"},

{6, "REFACTOR \n classes with union keywords, for better memory management"},
{6, "raii, sfinae, odr keywords, inline keywords"},

{6, "FEATURE \n Iterate over filenames image000...image991 and draw them one by one with some delay between"},

{6, "BACKLOG \n RendererBase::renderer_sdl_sw, renderer_sdl_hw, renderer_opengl, renderer_vk"},

{6, "OPENGL \n renderer_opengl, vect2,"
"Replace all SDL_Rects with vertex buffers (VBO) and vertex array objects (VAO)"
  "If some SDL_Rects were texctured, load textures into opengl, bind them and adjust texture coords"
  "If some SDL_Rects were colored, pass color data into a separate buffer or through uniform variables within your shader"
"Simple vertex and fragment shaders: For positioning and transformations pass a matrix to the shader"
  "Pass a matrix to the shader for position and transformation operations (scaling/translating/rotating)"
"Reduce number of draw calls when calling lots of rects by batching them into a single vbo"
  "Dynamically update the buffer or use instancing for rendering multiple rects in a single call"},

{6, "BACKLOG \n consider windows"},

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

#endif // ADDONS_H
