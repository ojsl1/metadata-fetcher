#include "main.h" // globals
#include "render.h" // -> drawFrame()
#include "input.h" // -> render.h -> <vector>
#include "button.h"
#include "addons.h"
#include "audio.h"
#include "png.h"
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
  // Open PNG file
  FILE* fp = fopen(filename, "rb");
  if (!fp){
    std::cerr << "Failed to open file: " << filename << std::endl;
  }

  // Init libpng structs
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png){
    std::cerr << "Failed to create PNG read struct" << std::endl;
    fclose(fp);
    return;
  }

  png_infop info = png_create_info_struct(png);
  if (!info){
    std::cerr << "Failed to create PNG info struct" << std::endl;
    png_destroy_read_struct(&png, NULL, NULL);
    fclose(fp);
    return;
  }

  if (setjmp(png_jmpbuf(png))){
    std::cerr << "libpng error!" << std::endl;
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return;
  }

  png_init_io(png, fp);

  // Read the PNG header
  png_read_info(png, info);

  // Print text metadata
  ReadTextChunks(png, info);
      
  //Cleanup
  png_destroy_read_struct(&png, &info, NULL);
  fclose(fp);
}

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
        case SDL_BUTTON_LEFT:
          if (buttonExit.hasintersection){
          currentMenu = MenuState::EXIT;
          }
        break;
      }
      break;

    case SDL_KEYDOWN:
      switch (e.key.keysym.sym){
        case SDLK_ESCAPE:
          currentMenu = MenuState::EXIT;
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

