#include "main.h" // sdl
#include "render.h"
#include "input.h"
#include "sprite.h"
#include "character.h"
#include "include/audio.h"
#include "font.h"
#include "menu.h"
#include "util.h"

#include <stdexcept>

AppContext gApp;
Font arial;
Mix_Chunk *bell;
Mix_Music *bgm;

// edge padding 10px
// inner padding 5px
// buttons 60x40
// button_wide 125x40
// infoframe 200x295
// window size see main() 320x480

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

MainMenuAssets mainmenuAssets{
    spriteExit,
    spriteTests,
    spriteDrop,
    spriteMute,
    spritePause,
    spriteBorder,
    spriteFrame,
    spriteBg,
    arial,
    player
};

MinigameAssets minigameAssets{
  spritePause,
  spritePlaceholder,
  arial,
  player2
};

void setupSpriteCallbacks()
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

void UpdateCollisionsMainMenu(Mouse &mouse)
{
  mouse.GetXY();

  // Update collisions between sprite-mouse
  DetectCollisions(mouse);
}

void UpdateCollisionsMinigame(Mouse &mouse)
{
  mouse.GetXY();

  // Update collisions between sprite-mouse
  DetectCollisions(mouse);

  // Update collisions between player-enemy
  //DetectCollisions(player2, enemy); //TODO DetectCollisions is still hardcoded for mouse
}

/* @brief Frame sequence pipeline
 * @comment In SDL-style double buffering the canonical frame sequence is:
 * 1. Handle input 2. Update gamestate (physics, collisions, etc.)
 * 3. Clear render target(s) 4. Draw everything 5. Present (swap buffers)
 * @warning The order "UpdateCollisons->Clear->Update->Render" only shows the recently Cleared buffer.
 */
void updateAndRenderMainMenu(RendererBase &ren,
                             Mouse &mouse,
                             const MainMenuAssets &mainmenuAssets)
{
  UpdateCollisionsMainMenu(mouse);
  ren.Clear();
  ren.RenderMainMenu(mouse,mainmenuAssets);
  ren.Update();
}

void updateAndRenderMinigame(RendererBase &ren,
                             Mouse &mouse,
                             const MinigameAssets &minigameAssets)
{
  UpdateCollisionsMinigame(mouse);
  ren.Clear();
  ren.RenderMinigame(mouse,minigameAssets);
  ren.Update();
}

void renderAppFrame(RendererBase &ren,
                    Mouse &mouse,
                    const MainMenuAssets &mainmenuAssets,
                    const MinigameAssets &minigameAssets)
{
  switch (gApp.mode) {
    case AppState::MAIN_MENU:
        updateAndRenderMainMenu(ren,mouse,mainmenuAssets);
        break;
    case AppState::MINIGAME:
        updateAndRenderMinigame(ren,mouse,minigameAssets);
        break;
    case AppState::EXIT:
        break;
    default:
        throw std::runtime_error("Undefined AppState from updateAndRenderAppState()!");
        break;
  }
}

void EventHandlerMainMenu(RendererBase &ren, Mouse &mouse, const SDL_Event &e)
{
  switch (e.type){

    case SDL_MOUSEBUTTONUP:
      switch (e.button.button){
        case SDL_BUTTON_LEFT:
          if (spriteExit.hasCollisions){
            gApp.mode = AppState::EXIT;
          } else if (spriteMute.hasCollisions){
              spriteMute.Toggle();
          } else if (spritePause.hasCollisions){
              spritePause.Toggle();
          } else if (spriteTests.hasCollisions){
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
          pngInfo = ExtractPNGInfo(droppedfile); SDL_Log("extractpnginfo success"); 
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

  std::cerr << "Initializing sprite callbacks..." << std::endl;
  setupSpriteCallbacks();

  std::cerr << "Loading media..." << std::endl;
  arial.Load("assets/arial.ttf", 24);

  // TODO moving below mouse definition out of main next to
  // other resources doesnt apply SDL_ShowCursor(false)
  Mouse mouse(24, 24, "assets/mouse.png");

  //Main loop flag
  gApp.mode = AppState::MAIN_MENU;
  
  std::cerr << "WIP Initializing menus..." << std::endl;
  MainMenuState mainMenu(ren,mouse);
  mainMenu.setBackground(&spriteBg);
  mainMenu.setFrame(&spriteFrame);

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
    player.Update(deltaTime);
    player2.Update(deltaTime);
    
    //DEBUG RECT:
    //std::cout << "Racer pos: " << player2.srcRect.x << ", " << player2.srcRect.y << std::endl;

    calc_framerate(lastTime,frameCount,gApp);

    renderAppFrame(ren,mouse,mainmenuAssets,minigameAssets);

    cap_framerate(currentTime);
  }

  arial.Shutdown();
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gApp);
  return 0;
}
