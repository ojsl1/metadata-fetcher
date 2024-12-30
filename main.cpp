#include "main.h" // globals
#include "render.h"
#include "input.h"
#include "button.h"
#include "addons.h"
#include "audio.h"
#include "png.h"

#include <iostream>
#include <fstream>
#include <vector>

enum class MenuState {
    MAIN_MENU,
    OPTIONS,
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

// edge padding 10px
// inner padding 5px
// button 60x40
// button wide 125x40
// window size is 480x320
Button buttonDrop(10, 10, 80, 80, "assets/dragndrop.png");
Button buttonMute(10, 180, 0, 0, "assets/button-mute.png");
Button buttonPause(75, 180, 0, 0, "assets/button-pause.png");
Button buttonTests(10, 225, 0, 0, "assets/button-tests.png");
Button buttonExit(10, 270, 0, 0, "assets/button-exit.png");
Button buttonFrame(0, 0, 0, 0, "assets/frame.png");

// Color definitions
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

void ReadTextChunks(png_structp png, png_infop info){
  int num_text = 0;
  png_textp text_data = NULL;
  
  // Retrieve the text chunks (tEXt, zTXt, iTXt)
  num_text = png_get_text(png, info, &text_data, &num_text);

  if (!num_text){
    std::cout << "No text metadata found in the png file." << std::endl;
    return;
  }

  //Loop through each text chunk
  for (int i = 0; i < num_text; ++i){
    std::cout << "Chunk key: " << text_data[i].key << std::endl;
    std::cout << "Text: " << text_data[i].text << std::endl;

    //Determine chunk type
    switch (text_data[i].compression){
      case PNG_TEXT_COMPRESSION_NONE:
        std::cout << "Chunk type: tEXt (uncompressed)" << std::endl;
        break;
      case PNG_TEXT_COMPRESSION_zTXt:
        std::cout << "Chunk type: TXt (compressed)" << std::endl;
        break;
      case PNG_ITXT_COMPRESSION_NONE:
      case PNG_ITXT_COMPRESSION_zTXt:
        std::cout << "Chunk type: zTXt (international text)" << std::endl;
        break;
      default:
        std::cout << "Chunk type: unknown" << std::endl;
    }
    std::cout << "-----------------------------------" << std::endl;
  }
}
void PrintPNGInfo(const char* filename){
  // std::ifstream automatically closes the file when it goes out of scope.
  // So no need to fclose(file) repeatedly.

  // Open the PNG file using std::ifstream
  std::ifstream file(filename, std::ios::binary);
  if (!file){
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // Read the file content into a vector
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // Init libpng structs
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png){
    std::cerr << "png_create_read_struct failed.\n" << std::endl;
    return;
  }

  png_infop info = png_create_info_struct(png);
  if (!info){
    std::cerr << "png_create_info_struct failed.\n" << std::endl;
    png_destroy_read_struct(&png, NULL, NULL);
    return;
  }

  if (setjmp(png_jmpbuf(png))){
    std::cerr << "libpng error!\n" << std::endl;
    png_destroy_read_struct(&png, &info, NULL);
    return;
  }

  /**
   * @brief Process the input buffer for libpng.
   * @details Read PNG data directly from the std::vector with a custom callback function
   * @param &buffer is the raw pointer that was passed to libpng; buf is a reference to the
   * dereferenced pointer retrieved from png_get_io_ptr(png).
   */
  png_set_read_fn(png, &buffer, [](png_structp png, png_bytep data, png_size_t length){
      auto &buf = *static_cast<std::vector<unsigned char>*>(png_get_io_ptr(png));
      std::copy(buf.begin(), buf.begin() + length, data);
      buf.erase(buf.begin(), buf.begin() + length);
      });

  // Read the PNG header
  png_read_info(png, info);

  // Print text metadata
  ReadTextChunks(png, info);

  // Get generics of the PNG
  png_uint_32 width, height;
  int bit_depth, color_type;

  png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
  std::cout << "PNG Info " << "Width: " << width << ", Height: " << height
            << ", Bit Depth: " << bit_depth << std::endl;
      
  //Cleanup
  png_destroy_read_struct(&png, &info, NULL);
}

void DetectIntersections(Mouse &mouse){
  // Interactable sprites need this, sets button.hasintersection accordingly
  buttonMute.DetectIntersections(mouse);
  buttonPause.DetectIntersections(mouse);
  buttonTests.DetectIntersections(mouse);
  buttonExit.DetectIntersections(mouse);
}

void UpdateInteractions(Mouse &mouse, SDL_Event &e){
  // MOUSE: Update the getters mouse coords
  mouse.GetXY();

  // MOUSE: Update button-mouse intersections
  DetectIntersections(mouse);
}

void renderMainMenuState(RendererBase &ren, Mouse &mouse, SDL_Event &e){
  UpdateInteractions(mouse, e);
  ren.Draw(mouse,buttonExit,buttonTests,buttonDrop,buttonMute,buttonPause,buttonFrame);
  ren.Present();
}

void renderStates(RendererBase &ren, Mouse &mouse, SDL_Event &e){
  switch (currentMenu) {
    case MenuState::MAIN_MENU:
        renderMainMenuState(ren,mouse,e);
        break;
    case MenuState::OPTIONS:
        //TODO renderOptionMenuState(...);
        break;
    case MenuState::EXIT:
        break;
    default:
        break;
  }
}

void EventHandlerMainMenu(RendererBase &ren, Mouse &mouse, const SDL_Event &e) {
  switch (e.type){
    case SDL_MOUSEBUTTONUP:
      switch (e.button.button){
        case SDL_BUTTON_LEFT:
          if (buttonExit.hasintersection){
            currentMenu = MenuState::EXIT;
          } else if (buttonMute.hasintersection){
            if (buttonMute.toggled){
              Mix_VolumeMusic(20);
              buttonMute.toggled = false;
              std::cout << "Mixer unmuted." << std::endl;
            } else {
              Mix_VolumeMusic(0);
              buttonMute.toggled = true;
              std::cout << "Mixer muted." << std::endl;
              std::cout << "mouse clicked at xy pos: " << mouse.point.x << "," << mouse.point.y
                        << " toggled was: " << buttonMute.toggled << std::endl;
              //std::cout << "got buttonMute wh bounds as: " << buttonMute.w << "," << buttonMute.h << std::endl;
            }
          } else if (buttonPause.hasintersection){
              if (buttonPause.toggled){
                Mix_ResumeMusic();
                buttonPause.toggled = false;
                std::cout << "Mixer resumed." << std::endl;
              } else {
                Mix_PauseMusic();
                buttonPause.toggled = true;
                std::cout << "Mixer pause." << std::endl;
              }
          }
          break;

        default:
        break;
      }
      break;

    case SDL_KEYDOWN:
      switch (e.key.keysym.sym){
        case SDLK_ESCAPE:
          currentMenu = MenuState::EXIT;
          break;
        case SDLK_m:
          Mix_VolumeMusic(0);
          std::cout << "Mixer muted via m." << std::endl; //TODO needs to be toggleable
          break;
      }
      break;

    case SDL_DROPFILE: {
      char* libpng_droppedfile = e.drop.file;

      if (libpng_droppedfile && libpng_droppedfile[0] != '\0') {
          std::cout << "File dropped: " << libpng_droppedfile << std::endl;
          PrintPNGInfo(libpng_droppedfile);

          SDL_free(libpng_droppedfile);
      }else{
          std::cerr << "Error: Dropped file was null or empty." << std::endl;
      }
      break;
    }
  } 
}

void EventHandlerGlobal(RendererBase &ren, Mouse &mouse, SDL_Event &e){
  // Global event handling for the windows X button
  if (e.type == SDL_QUIT){
    currentMenu = MenuState::EXIT;
    return;
  }

  // Global event handling for Ctrl+q
  if (e.type == SDL_KEYDOWN){
    if ((e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_q) && 
        (e.key.keysym.mod & KMOD_CTRL)) {
        currentMenu = MenuState::EXIT;
        return;
    }
  }

  // State-specific event handling
  switch (currentMenu) {
    case MenuState::MAIN_MENU:
        EventHandlerMainMenu(ren,mouse,e);
        break;
    case MenuState::OPTIONS:
        //TODO EventHandlerOptionsMenu(...);
        std::cout << "Not implemented, exiting..." << std::endl;
        currentMenu = MenuState::EXIT;
        break;
    case MenuState::EXIT:
        break;
    default:
        break;
  }
}

int main (int argc, char *argv[]){
  RendererBase ren;
  Audio audio;

  ren.initVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
  ren.initColors(gScreen);
  audio.initMixer();

  Uint32 starting_tick;
  currentMenu = MenuState::MAIN_MENU;

  Mouse mouse(24, 24, "assets/mouse00.png"); // TODO moving this out of main next to the other resources doesnt apply SDL_ShowCursor(false)
  
  while (currentMenu != MenuState::EXIT){
    starting_tick = SDL_GetTicks();

    SDL_Event e;
    while (SDL_PollEvent(&e)){
        EventHandlerGlobal(ren,mouse,e); // handle events for the current state
    }

    renderStates(ren,mouse,e); // render the current state
    ren.cap_framerate(starting_tick);
  }
 
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gWindow, dims);
  return 0;
}

