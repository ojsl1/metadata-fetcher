#include "main.h" // sdl
#include "render.h"
#include "input.h"
#include "sprite.h"
#include "character.h"
#include "include/audio.h"
#include "font.h"
#include "png.h"

#include <iostream>
#include <fstream>
#include <vector>

// System
AppState currentMenu = AppState::MAIN_MENU;
WindowDimensions dims;
float fps = 0.0f;
// Resources
Font arial;
Mix_Chunk *bell;
Mix_Music *bgm;
// Colors
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

// edge padding 10px
// inner padding 5px
// buttons 60x40
// button_wide 125x40
// infoframe 200x295
// window size see int main 320x480

//x,y,w,h
// ("name", drawcoords, drawsize, "filepath", spritesheet coordinates)
//NB: Only DrawScaled can use custom drawing size
Sprite spriteBg("Background", 0, 0, 320, 480, "assets/spritesheet.png", {0,280,480,320});
Sprite spriteBorder("Border", 0, 0, 320, 480, "assets/spritesheet.png", {480,280,480,320});
Sprite spriteFrame("Frame", 10, 10, 300, 410, "assets/spritesheet.png", {960,280,200,295});
Sprite spriteDrop("Drop", 120, 180, 80, 140, "assets/spritesheet.png", {0,80,120,198});

Sprite spritePause("Pause", 10, 430, 0, 0, "assets/spritesheet.png", {60,0,60,40});
Sprite spriteMute("Mute", 90, 430, 0, 0, "assets/spritesheet.png", {0,0,60,40});
Sprite spriteTests("Tests", 170, 430, 0, 0, "assets/spritesheet.png", {180,0,60,40});
Sprite spriteExit("Exit", 250, 430, 0, 0, "assets/spritesheet.png", {120,0,60,40});

Character player("Marisa", 0, 240, 192, 192, "assets/marisa-antinomy-sheet.png", {1,648,192,192},15);


void initSprites(){
  spriteMute.SetToggleCallback([](bool toggled){
      if(toggled){
        Mix_VolumeMusic(0);
        std::cout << "Mixer muted." << std::endl;
        //std::cout << "mouse clicked at xy pos: " << mouse.point.x << "," << mouse.point.y
        //          << " toggled was: " << spriteMute.toggled << std::endl;
        //std::cout << "got spriteMute wh bounds as: " << spriteMute.w << "," << spriteMute.h << std::endl;
      } else {
        Mix_VolumeMusic(20);
        std::cout << "Mixer unmuted." << std::endl;
      }
  });
  
  spritePause.SetToggleCallback([](bool toggled){
      if(toggled){
        Mix_PauseMusic();
        std::cout << "Mixer paused." << std::endl;
      } else {
        Mix_ResumeMusic();
        std::cout << "Mixer resumed." << std::endl;
      }
  });
}

void ReadTextChunks(png_structp png, png_infop info){
  int num_text = 0;
  png_textp text_data = nullptr;
  
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

/* @brief
 *
 * @comment std::ifstream automatically closes the file when it goes out of scope.
 * So no need to fclose(file) repeatedly.
 */
void PrintPNGInfo(const char* filename){
  // Open the PNG file using std::ifstream
  std::ifstream file(filename, std::ios::binary);
  if (!file){
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  // Read the file content into a vector
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // Init libpng structs
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png){
    std::cerr << "png_create_read_struct failed.\n" << std::endl;
    return;
  }

  png_infop info = png_create_info_struct(png);
  if (!info){
    std::cerr << "png_create_info_struct failed.\n" << std::endl;
    png_destroy_read_struct(&png, nullptr, nullptr);
    return;
  }

  if (setjmp(png_jmpbuf(png))){
    std::cerr << "libpng error!\n" << std::endl;
    png_destroy_read_struct(&png, &info, nullptr);
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

  png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
  std::cout << "PNG Info " << "Width: " << width << ", Height: " << height
            << ", Bit Depth: " << bit_depth << std::endl;
      
  //Cleanup
  png_destroy_read_struct(&png, &info, nullptr);
}

void DetectIntersections(Mouse &mouse){
  // Interactable sprites need this, sets sprite.hasintersection accordingly
  spriteMute.DetectIntersections(mouse);
  spritePause.DetectIntersections(mouse);
  spriteTests.DetectIntersections(mouse);
  spriteExit.DetectIntersections(mouse);
}

void UpdateInteractions(Mouse &mouse, SDL_Event &e){
  // MOUSE: Update the getters mouse coords
  mouse.GetXY();

  // MOUSE: Update sprite-mouse intersections
  DetectIntersections(mouse);
}

void renderMainAppState(RendererBase &ren, Mouse &mouse, SDL_Event &e){
  UpdateInteractions(mouse, e);
  ren.Clear();
  ren.Render(mouse,spriteExit,spriteTests,spriteDrop,spriteMute,
           spritePause,spriteBorder,spriteFrame,
           spriteBg,arial,player);
  ren.Update();
}

void renderState(RendererBase &ren, Mouse &mouse, SDL_Event &e){
  switch (currentMenu) {
    case AppState::MAIN_MENU:
        renderMainAppState(ren,mouse,e);
        break;
    case AppState::MINIGAME:
        renderMainAppState(ren,mouse,e);
        break;
    case AppState::EXIT:
        break;
    default:
        break;
  }
}

void EventHandlerMainMenu(RendererBase &ren, Mouse &mouse, const SDL_Event &e) {
  switch (e.type){

    case SDL_WINDOWEVENT:
      switch (e.window.event){
        case SDL_WINDOWEVENT_RESIZED:
          //std::cout << "resize detected: " << e.window.data1 << "x" << e.window.data2 << std::endl;

          // Reacquire window surface if it becomes invalid
          gScreen = SDL_GetWindowSurface(gWindow);
          if (!gScreen){
            std::cerr << "SDL_GetWindowSurface failed during resize: "
                      << SDL_GetError() << std::endl;
          }
          break;

        default:
          break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch (e.button.button){
        case SDL_BUTTON_LEFT:
          if (spriteExit.hasintersection){
            currentMenu = AppState::EXIT;
          } else if (spriteMute.hasintersection){
              spriteMute.Toggle();
          } else if (spritePause.hasintersection){
              spritePause.Toggle();
          }
          break;

        default:
        break;
      }
      break;

    case SDL_KEYDOWN:
      switch (e.key.keysym.sym){
        case SDLK_ESCAPE:
          currentMenu = AppState::EXIT;
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
  // Global event handling for the windows X sprite
  if (e.type == SDL_QUIT){
    currentMenu = AppState::EXIT;
    return;
  }

  // Global event handling for Ctrl+q
  if (e.type == SDL_KEYDOWN){
    if ((e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_q) && 
        (e.key.keysym.mod & KMOD_CTRL)) {
        currentMenu = AppState::EXIT;
        return;
    }
  }

  // State-specific event handling
  switch (currentMenu) {
    case AppState::MAIN_MENU:
        EventHandlerMainMenu(ren,mouse,e);
        break;
    case AppState::MINIGAME:
        static bool printed=false;
          if(!printed){
          std::cerr << "TODO: EventHandlerMainMenu() -> EventHandlerMinigame()" << std::endl;
          std::cerr << "TODO: switching appstate -> dealloc sprites, bgm, bell etc properly" << '\n';
          printed=true;
          }
        EventHandlerMainMenu(ren,mouse,e);
        ren.main = false;
        break;
    case AppState::EXIT:
        break;
    default:
        break;
  }
}

void handleRtInput(Character &player){
  //State based input for player movement and continuous controls
  const Uint8* keys = SDL_GetKeyboardState(NULL);

  if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) player.move(0, -1);
  if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) player.move(0, 1);
  if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) { player.move(-2, 0); player.playAnimation(AnimationState::MOVE_BACK, 500); };
  if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) { player.move(2, 0); player.playAnimation(AnimationState::MOVE_FORWARD, 500); };
  if (keys[SDL_SCANCODE_Q]) player.playAnimation(AnimationState::ATTACK_UP_B, 4000);
  if (keys[SDL_SCANCODE_E]) player.playAnimation(AnimationState::ATTACK_B, 4000);
  if (keys[SDL_SCANCODE_1]) player.playAnimation(AnimationState::IDLE, 5000);


}

/**
 * @brief Calculates the FPS at regular intervals.
 *
 * Computes the frame rate by counting the number of frames rendered 
 * within a specified time window (100 ms). It resets the counter and updates the 
 * reference time to ensure steady measurement interval.
 *
 * @param lastTime Reference to the last recorded time (in milliseconds).
 * @param frameCount Reference to the frame count within the interval.
 * @param fps Reference to the FPS value to be updated.
 *
 * @comment The logic ensures that `lastTime` is only updated when an FPS calculation occurs, 
 * preventing constant resets and allowing a stable frame rate measurement.
 */
void calc_framerate(Uint32 &lastTime, Uint32 &frameCount, float &fps){
  Uint32 currentTime = SDL_GetTicks();

  if (currentTime - lastTime >= 100){ // every 100ms
    fps = frameCount / ((currentTime - lastTime) / 1000.0f);
    frameCount = 0; // reset framecounter for the next iteration
    lastTime = currentTime; // update reference time
  }
}

/**
 * @brief Caps the frame rate to a specified FPS limit.
 *
 * Ensures that the frame rate does not exceed the FPSCAP, specified in main.h,
 * by calculating the time taken for the current frame and introducing a delay 
 * if necessary. The delay ensures that the frame rate remains within the desired 
 * limit, preventing the application from running too fast.
 *
 * @param currentTime Reference to the current time (ms) when the frame rendering
 * started.
 *
 * @comment The logic compares the time taken for the current frame with the target 
 * FPS limit, and introduces a delay if the frame was rendered too quickly, ensuring 
 * the frame rate is capped at `FPSCAP`.
 */
void cap_framerate(Uint32 &currentTime){
  Uint32 frameTime = SDL_GetTicks() - currentTime;
  if (frameTime < (1000 / FPSCAP)) {
    SDL_Delay((1000 / FPSCAP) - frameTime);
  }
}

int main (int argc, char *argv[]){
  std::cerr << "Initializing renderer..." << std::endl;
  RendererBase ren;
  ren.initVideo(320,480);
  ren.initColors(gScreen);

  std::cerr << "Initializing audio subsystem..." << std::endl;
  Audio audio;
  audio.initMixer();

  std::cerr << "Initializing sprite states..." << std::endl;
  initSprites();

  std::cerr << "Loading media..." << std::endl;
  arial.Load("assets/arial.ttf", 24);

  // TODO moving below mouse definition out of main next to
  // other resources doesnt apply SDL_ShowCursor(false)
  Mouse mouse(24, 24, "assets/mouse00.png");

  //Main loop flag
  currentMenu = AppState::MAIN_MENU;

  //Event handler
  SDL_Event e;

  //Time tracking
  Uint32 lastTime = SDL_GetTicks(); //Initialize lasttime (ms)
  Uint32 frameCount = 0;

  //Output environment info
  std::cerr << "FPSCAP is set to: " << FPSCAP << std::endl;
  
  audio.playMusic();
  spritePause.Toggle();

  //While app is running
  while (currentMenu != AppState::EXIT){
    frameCount++;
    Uint32 currentTime = SDL_GetTicks(); // current loop iteration time (ms)
    
    //Calculate deltatime ie.tickratee for animations:
    //ie. how long it took for last loop iter to reach this iter
    double deltaTime = static_cast<double>(currentTime - lastTime);

    //Event queue
    while (SDL_PollEvent(&e)){
        EventHandlerGlobal(ren,mouse,e); //one time events
    }

    handleRtInput(player); //use sdl_getkeyboardstate
    player.update(deltaTime);

    calc_framerate(lastTime,frameCount,fps);

    //Order: UpdateInteractions->Clear->Render->Update
    renderState(ren,mouse,e);

    cap_framerate(currentTime);
  }

  arial.Shutdown();
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gWindow, dims);
  return 0;
}
