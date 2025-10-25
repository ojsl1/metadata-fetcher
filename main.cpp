#include "main.h" // sdl
#include "render.h"
#include "input.h"
#include "sprite.h"
#include "character.h"
#include "include/audio.h"
#include "font.h"
#include "png.h"

#include <fstream>
#include <stdexcept>
#include <vector>

// System
AppContext gApp;
// Resources
Font arial;
Mix_Chunk *bell;
Mix_Music *bgm;

// edge padding 10px
// inner padding 5px
// buttons 60x40
// button_wide 125x40
// infoframe 200x295
// window size see main() 320x480

/**
 * @brief Sprite(spriteName, x, y, w, h, spritesheetPath, spriteRect)
 *        Only DrawScaled() can use the drawing size values
 * @param x,y: drawing coordinates
 *        w,h: drawing size
 *        spriteRect: coordinates and size on the spritesheetPath(x,y,w,h)
 */
Sprite spritePlaceholder("Placeholder", 0, 0, 1009, 697, "assets/race-placeholder.png", {0,0,1009,697});
Sprite spriteBg("Background", 0, 0, 320, 480, "assets/spritesheet.png", {0,280,480,320});
Sprite spriteBorder("Border", 0, 0, 320, 480, "assets/spritesheet.png", {480,280,480,320});
Sprite spriteFrame("Frame", 10, 10, 300, 410, "assets/spritesheet.png", {960,280,200,295});
Sprite spriteDrop("Drop", 120, 180, 80, 140, "assets/spritesheet.png", {0,80,120,198});

Sprite spritePause("Pause", 10, 430, 0, 0, "assets/spritesheet.png", {60,0,60,40});
Sprite spriteMute("Mute", 90, 430, 0, 0, "assets/spritesheet.png", {0,0,60,40});
Sprite spriteTests("Tests", 170, 430, 0, 0, "assets/spritesheet.png", {180,0,60,40});
Sprite spriteExit("Exit", 250, 430, 0, 0, "assets/spritesheet.png", {120,0,60,40});

auto animsdef1 = Character::loadAnimationConfig("assets/data-marisa-antinomy.json", "Marisa");
auto animsdef2 = Character::loadAnimationConfig("assets/data-racer.json", "Racer");
Character player("Marisa", 0, 240, animsdef1);
Character player2("Racer", 0, 240, animsdef2);

void initSprites()
{
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

PNGDisplayInfo ExtractPNGInfo(const char* filename)
{
  PNGDisplayInfo out;

  /* ---------- read file into memory ---------- */
  std::ifstream file(filename, std::ios::binary);
  if (!file)                 // failure → empty object
      return out;

  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                                    std::istreambuf_iterator<char>());

  /* ---------- libpng setup ---------- */
  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                           nullptr, nullptr, nullptr);
  if (!png)
      return out;

  png_infop info = png_create_info_struct(png);
  if (!info) {
      png_destroy_read_struct(&png, nullptr, nullptr);
      return out;
  }

  if (setjmp(png_jmpbuf(png))) {           // error path
      png_destroy_read_struct(&png, &info, nullptr);
      return out;
  }

  /* feed libpng from the in‑memory buffer */
  png_set_read_fn(
      png, &buffer,
      [](png_structp png_ptr, png_bytep dst, png_size_t len)
      {
          auto& buf = *static_cast<std::vector<unsigned char>*>(
                         png_get_io_ptr(png_ptr));
          std::copy(buf.begin(), buf.begin() + len, dst);
          buf.erase(buf.begin(), buf.begin() + len);
      });

  png_read_info(png, info);

  // ---------------- text chunks -------------
  int num_text = 0;
  png_textp text_data = nullptr;
  num_text = png_get_text(png, info, &text_data, &num_text);

  if (!num_text){
    std::cout << "No text metadata found in the png file." << std::endl;
  }

  //Loop through each text chunk
  for (int i = 0; i < num_text; ++i){
    out.lines.emplace_back("Chunk key: "  + std::string(text_data[i].key));
    out.lines.emplace_back("Text: "       + std::string(text_data[i].text));

    //Determine chunk type
    switch (text_data[i].compression){
      case PNG_TEXT_COMPRESSION_NONE:
        out.lines.emplace_back("Chunk type: tEXt (uncompressed)");
        break;
      case PNG_TEXT_COMPRESSION_zTXt:
        out.lines.emplace_back("Chunk type: TXt (compressed)");
        break;
      case PNG_ITXT_COMPRESSION_NONE:
      case PNG_ITXT_COMPRESSION_zTXt:
        out.lines.emplace_back("Chunk type: zTXt (international text)");
        break;
      default:
        out.lines.emplace_back("Chunk type: unknown");
    }
  }
    out.lines.emplace_back("-----------------------------------");

    png_get_IHDR(png, info, &out.width, &out.height, &out.bitDepth, nullptr, nullptr, nullptr, nullptr);
    /*std::cout << "PNG Info " << "Width: " << out.width << ", Height: " << out.height
              << ", Bit Depth: " << out.bitDepth << std::endl;
              */
        
    out.lines.insert(out.lines.begin(),
      "PNG Info W:" + std::to_string(out.width)  +
      "  H:"        + std::to_string(out.height) +
      "  BitDepth:" + std::to_string(out.bitDepth));

    //Cleanup
    png_destroy_read_struct(&png, &info, nullptr);
    return out;
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

void DetectCollisions(Mouse &mouse)
{
  // Interactable sprites need this, sets sprite.hasCollision accordingly
  switch (gApp.mode){
    case AppState::MAIN_MENU:
      spriteMute.DetectCollisions(mouse);
      spritePause.DetectCollisions(mouse);
      spriteTests.DetectCollisions(mouse);
      spriteExit.DetectCollisions(mouse);
      break;
    case AppState::MINIGAME:
      //playerCar.DetectCollisions(player, enemy);
      break;
    default:
      throw std::invalid_argument("Undefined AppState from DetectCollisions()");
      break;
  }
}

void UpdateCollisions(Mouse &mouse)
{
  mouse.GetXY();

  // Update collisions between sprite-mouse
  DetectCollisions(mouse);

  // Update collisions between player-enemy
  //DetectCollisions(player, enemy); //TODO DetectCollisions is still hardcoded for mouse
}

void renderMenus(RendererBase &ren, Mouse &mouse)
{
  UpdateCollisions(mouse);
  ren.Clear();
  ren.Render(mouse,spriteExit,spriteTests,spriteDrop,spriteMute,
           spritePause,spriteBorder,spriteFrame,
           spriteBg,spritePlaceholder,arial,player,player2);
  ren.Update();
}

void renderState(RendererBase &ren, Mouse &mouse)
{
  switch (gApp.mode) {
    case AppState::MAIN_MENU:
        renderMenus(ren,mouse);
        break;
    case AppState::MINIGAME:
        renderMenus(ren,mouse);
        break;
    case AppState::EXIT:
        break;
    default:
        throw std::invalid_argument("Undefined AppState from renderState()!");
        break;
  }
}

void EventHandlerMainMenu(RendererBase &ren, Mouse &mouse, const SDL_Event &e)
{
  switch (e.type){

    case SDL_MOUSEBUTTONUP:
      switch (e.button.button){
        case SDL_BUTTON_LEFT:
          if (spriteExit.hascollisions){
            gApp.mode = AppState::EXIT;
          } else if (spriteMute.hascollisions){
              spriteMute.Toggle();
          } else if (spritePause.hascollisions){
              spritePause.Toggle();
          } else if (spriteTests.hascollisions){
              std::cout << "unimplemented" << '\n';
              gApp.mode = AppState::MINIGAME;
          }break;
        //default: throw std::invalid_argument("Undefined SDL_MOUSEBUTTONUP event!"); break;
      }break;

    case SDL_KEYDOWN:
      switch (e.key.keysym.sym){
        case SDLK_ESCAPE:
          gApp.mode = AppState::EXIT;
          break;
        case SDLK_m:
          Mix_VolumeMusic(0);
          std::cerr << "Mixer muted via m." << std::endl;
          break;
        //default: throw std::invalid_argument("Undefined SDL_KEYDOWN event!"); break;
        }break;

    case SDL_DROPFILE:{
      char* droppedfile = e.drop.file;
      if (droppedfile && droppedfile[0] != '\0') {
          std::cout << "File dropped: " << droppedfile << std::endl;
          PrintPNGInfo(droppedfile); SDL_Log("printpnginfo success"); 
          gApp.pngInfo = ExtractPNGInfo(droppedfile); SDL_Log("extractpnginfo success"); 
          SDL_free(droppedfile);
      }else{
          std::cerr << "Error: Dropped file was null or empty." << std::endl; }
      break;}
  }
}

void EventHandlerGlobal(RendererBase &ren, Mouse &mouse, SDL_Event &e)
{
  //Reacquire window surface if it becomes invalid
  if (e.window.event == SDL_WINDOWEVENT_RESIZED){
    gApp.screen = SDL_GetWindowSurface(gApp.windowHandle);
    if (!gApp.screen){ std::cerr << "SDL_GetWindowSurface failed durig resize: "
                             << SDL_GetError() << std::endl;
    }
  }

  // X
  if (e.type == SDL_QUIT){
    gApp.mode = AppState::EXIT;
    return;
  }

  // Ctrl+q
  if (e.type == SDL_KEYDOWN){
    if ((e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_q) && 
        (e.key.keysym.mod & KMOD_CTRL)) {
        gApp.mode = AppState::EXIT;
        return;
    }
  }

  switch (gApp.mode) {
    case AppState::MAIN_MENU:
        EventHandlerMainMenu(ren,mouse,e);
        break;
    case AppState::MINIGAME:
        break;
    case AppState::EXIT:
        break;
    default: throw std::invalid_argument("Undefined appstate!"); break;
  }
}

void handleRealtimeInput(Character &player, Character &player2)
{
  // Player movement and continuous controls
  // TODO playAnimation shouldnt be called when player is racer
  const Uint8* keys = SDL_GetKeyboardState(NULL);

  if (keys[SDL_SCANCODE_W]) player.move(0, -1);
  if (keys[SDL_SCANCODE_S]) player.move(0, 1);
  if (keys[SDL_SCANCODE_A]) { player.move(-2, 0); player.playAnimation(AnimationState::MOVE_BACK, 500); };
  if (keys[SDL_SCANCODE_D]) { player.move(2, 0); player.playAnimation(AnimationState::MOVE_FORWARD, 500); };
  if (keys[SDL_SCANCODE_Q]) player.playAnimation(AnimationState::ATTACK_UP_B, 4000);
  if (keys[SDL_SCANCODE_E]) player.playAnimation(AnimationState::ATTACK_B, 4000);
  if (keys[SDL_SCANCODE_1]) player.playAnimation(AnimationState::IDLE, 5000);

  if (keys[SDL_SCANCODE_UP]) {player2.move(0, -1); std::cout << "moved up" << '\n';};
  if (keys[SDL_SCANCODE_DOWN]) {player2.move(0, 1); std::cout << "moved down" << '\n';};
  if (keys[SDL_SCANCODE_LEFT]) {player2.move(-2, 0); std::cout << "moved left" << '\n';};
  if (keys[SDL_SCANCODE_RIGHT]) {player2.move(2, 0); std::cout << "moved right" << '\n';};
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
void calc_framerate(Uint32 &lastTime, Uint32 &frameCount, AppContext &ctx)
{
  Uint32 currentTime = SDL_GetTicks();

  if (currentTime - lastTime >= 100){ // every 100ms
    ctx.fps = frameCount / ((currentTime - lastTime) / 1000.0f);
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
void cap_framerate(Uint32 &currentTime)
{
  Uint32 frameTime = SDL_GetTicks() - currentTime;
  if (frameTime < (1000 / FPSCAP)) {
    SDL_Delay((1000 / FPSCAP) - frameTime);
  }
}

int main (int argc, char *argv[])
{
  std::cerr << "Initializing renderer..." << std::endl;
  RendererBase ren;
  ren.initVideo(320,480);
  ren.initColors(gApp);

  std::cerr << "Initializing audio subsystem..." << std::endl;
  Audio audio;
  audio.initMixer();

  std::cerr << "Initializing sprite states..." << std::endl;
  initSprites();

  std::cerr << "Loading media..." << std::endl;
  arial.Load("assets/arial.ttf", 24);

  // TODO moving below mouse definition out of main next to
  // other resources doesnt apply SDL_ShowCursor(false)
  Mouse mouse(24, 24, "assets/mouse.png");

  //Main loop flag
  gApp.mode = AppState::MAIN_MENU;

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
  while (gApp.mode != AppState::EXIT){
    frameCount++;
    Uint32 currentTime = SDL_GetTicks(); // current loop iteration time (ms)
    
    //Calculate deltatime ie.tickratee for animations:
    //ie. how long it took for last loop iter to reach this iter
    double deltaTime = static_cast<double>(currentTime - lastTime);

    //Event queue
    while (SDL_PollEvent(&e)){
        EventHandlerGlobal(ren,mouse,e); //one time events
    }

    handleRealtimeInput(player,player2); //use sdl_getkeyboardstate
    player.update(deltaTime);
    player2.update(deltaTime);
    
    //DEBUG RECT:
    //std::cout << "Racer pos: " << player2.srcRect.x << ", " << player2.srcRect.y << std::endl;

    calc_framerate(lastTime,frameCount,gApp);

    //Order: UpdateCollisions->Clear->Render->Update
    renderState(ren,mouse);

    cap_framerate(currentTime);
  }

  arial.Shutdown();
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gApp);
  return 0;
}
