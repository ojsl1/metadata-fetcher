#include "main.h" // sdl
#include "SDL_events.h"
#include "render.h"
#include "input.h"
#include "sprite.h"
#include "character.h"
#include "include/audio.h"
#include "font.h"
#include "menu.h"
#include "util.h"

#include <cstring>
#include <stdexcept>

AppContext gApp;
Font arial;
Mix_Chunk *bell;
Mix_Music *bgm;


//Time tracking
Uint32 lastFpsTime = 0; //fps measurement timing
Uint32 lastDeltaTime = 0; //simulation timing
Uint32 frameCount = 0;

// edge padding 10px
// inner padding 5px
// buttons 60x40
// button_wide 125x40
// infoframe 200x295
// window size see main() 320x480

Mouse mouse(24, 24, "assets/mouse.png");
Sprite spritePlaceholder("Placeholder", 0, 0, 1009, 697, "assets/race-placeholder.png", {0,0,1009,697});
Sprite spriteBg("Background", 0, 0, 320, 480, "assets/spritesheet.png", {0,280,480,320});
Sprite spriteBorder("Border", 0, 0, 320, 480, "assets/spritesheet.png", {480,280,480,320});
Sprite spriteFrame("Frame", 10, 10, 300, 410, "assets/spritesheet.png", {960,280,200,295});
Sprite spriteDrop("Drop", 120, 180, 80, 140, "assets/spritesheet.png", {0,80,120,198});

Sprite spritePause("Pause", 10, 430, 0, 0, "assets/spritesheet.png", {60,0,60,40});
Sprite spriteMute("Mute", 90, 430, 0, 0, "assets/spritesheet.png", {0,0,60,40});
Sprite spriteTests("Tests", 170, 430, 0, 0, "assets/spritesheet.png", {180,0,60,40});
Sprite spriteExit("Exit", 250, 430, 0, 0, "assets/spritesheet.png", {120,0,60,40});

auto animsdef1 = Character::loadAnimationConfig("Marisa", "assets/data-marisa-antinomy.json");
auto animsdef2 = Character::loadAnimationConfig("Racer", "assets/data-racer.json");
Character player("Marisa", 0, 240, animsdef1);
Character player2("Racer", 0, 240, animsdef2);

MainMenuAssets mainmenuAssets{
    &spriteExit,
    &spriteTests,
    &spriteDrop,
    &spriteMute,
    &spritePause,
    &spriteBorder,
    &spriteFrame,
    &spriteBg,
    &arial,
    &player
};

MinigameAssets minigameAssets{
  &spritePause,
  &spritePlaceholder,
  &arial,
  &player2
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
  // --- Interactable sprites need this ---
  // These are AppState-specific collisions, but sprites themselves are/act
  // AppState-specific (except mouse). TODO Make a bool for this in Sprite,
  // so you can easily set these during construction. Or, is it possible?
  // You might need to abstract DetectCollisions(x,y,z) more. Or, make it
  // so the collision pairs are also defined on construction.
  switch (gApp.mode){
    case AppState::MAIN_MENU:
      spriteMute.DetectCollisions(mouse);
      spritePause.DetectCollisions(mouse);
      spriteTests.DetectCollisions(mouse);
      spriteExit.DetectCollisions(mouse);
      break;
    case AppState::MINIGAME:
      //player2.DetectCollisions(player, enemy);
      break;
    default: throw std::invalid_argument("Undefined AppState from DetectCollisions()"); break;
  }
}

void UpdateCollisions(Mouse &mouse)
{
  switch(gApp.mode)
  {
    case AppState::MAIN_MENU:
      mouse.GetXY();
      // Update collisions between sprite-mouse
      DetectCollisions(mouse);
      break;

    case AppState::MINIGAME:
      mouse.GetXY();
      // Update collisions between sprite-mouse
      DetectCollisions(mouse);
      // Update collisions between player-enemy
      //DetectCollisions(player2, enemy); //TODO DetectCollisions is still hardcoded for mouse
      break;

    case AppState::EXIT:
      break;
    default: throw std::invalid_argument("Undefined appstate from UpdateCollisions()"); break;
  }
}

void ListenMainMenuEvents(IRenderer &ren, Mouse &mouse, const SDL_Event &e)
{
  switch(e.type)
  {
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
              gApp.mode = AppState::MINIGAME;
          }break;
        //default: throw std::invalid_argument("Undefined SDL_MOUSEBUTTONUP event!"); break;
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

void ListenGlobalKeys(IRenderer &ren, Mouse &mouse, SDL_Event &e)
{
  //Reacquire window surface if it becomes invalid
  if (e.window.event == SDL_WINDOWEVENT_RESIZED){
    gApp.screen = SDL_GetWindowSurface(gApp.win);
    if (!gApp.screen){ std::cerr << "SDL_GetWindowSurface failed durig resize: "
                             << SDL_GetError() << std::endl;
    }
  }

  // X
  if (e.type == SDL_QUIT){
    gApp.mode = AppState::EXIT;
    return;
  }

  if (e.type == SDL_KEYUP){
    // Ctrl+q
    if ((e.key.keysym.sym == SDLK_q) && (e.key.keysym.mod & KMOD_CTRL)){
        gApp.mode = AppState::EXIT;
        return;
    }
    // Esc
    if (e.key.keysym.sym == SDLK_ESCAPE){
        gApp.mode = AppState::EXIT;
        return;
    }
    // m
    if (e.key.keysym.sym == SDLK_m){
        Mix_VolumeMusic(0);
        //TODO: Set mainmenu MUTE sprite to UNMUTE
        std::cerr << "Mixer muted via m." << std::endl;
    }
  }

  switch (gApp.mode)
  {
    case AppState::MAIN_MENU:
        ListenMainMenuEvents(ren,mouse,e);
        break;
    case AppState::MINIGAME:
        break;
    case AppState::EXIT:
        break;
    default: throw std::invalid_argument("Undefined appstate from ListenGlobalKeys()!"); break;
  }
}

void ListenGameKeys(Character &player, Character &player2)
{
  // Player movement and continuous controls
  // TODO playAnimation shouldnt be called when player is racer
  const Uint8* keys = SDL_GetKeyboardState(NULL);

  if (keys[SDL_SCANCODE_W]) player.move(0, -1);
  if (keys[SDL_SCANCODE_S]) player.move(0, 1);
  if (keys[SDL_SCANCODE_A]) { player.move(-2, 0); player.playAnimation(AnimationState::MOVE_BACK, 10); };
  if (keys[SDL_SCANCODE_D]) { player.move(2, 0); player.playAnimation(AnimationState::MOVE_FORWARD, 10); };
  if (keys[SDL_SCANCODE_Q]) /*{player.attack_up_b(?)};*/ player.playAnimation(AnimationState::ATTACK_UP_B, 50);
  if (keys[SDL_SCANCODE_E]) /*{player.attack_b(?)};*/player.playAnimation(AnimationState::ATTACK_B, 50);
  if (keys[SDL_SCANCODE_1]) /*{player.cleanse_state(?)};*/player.playAnimation(AnimationState::IDLE, 0);

  if (keys[SDL_SCANCODE_UP]) player2.move(0, -1);;
  if (keys[SDL_SCANCODE_DOWN]) player2.move(0, 1);;
  if (keys[SDL_SCANCODE_LEFT]) player2.move(-2, 0);;
  if (keys[SDL_SCANCODE_RIGHT]) player2.move(2, 0);;
}

/**
 * @brief Calculates the FPS at regular intervals.
 *
 * Computes the frame rate by counting the number of frames rendered 
 * within a specified time window (100 ms). It resets the counter and updates the 
 * reference time to ensure steady measurement interval.
 *
 * @param lastFpsTime Reference to the last recorded time (in milliseconds).
 * @param frameCount Reference to the frame count within the interval.
 * @param fps Reference to the FPS value to be updated.
 *
 * @comment The logic ensures that `lastFpsTime` is only updated when an FPS calculation occurs, 
 * preventing constant resets and allowing a stable frame rate measurement.
 */
void calc_framerate(Uint32 &lastFpsTime, Uint32 &frameCount, AppContext &ctx)
{
  Uint32 currentTime = SDL_GetTicks64();
  Uint32 elapsed = currentTime - lastFpsTime;

  if (elapsed >= 100) // every 100ms
  {
    ctx.fps = frameCount / (elapsed / 1000.0f);

    frameCount = 0; // reset framecounter for the next iteration
    lastFpsTime = currentTime; // update reference time
  }
}

/**
 * @brief cap framerate to FPSCAP
 *
 * Calculate the time taken for the current frame and if necessary introduce
 * a delay using SDL_Delay. SDL_Delay takes Uint32, not double,
 * this is what leads to the loss of precision i.e. 60.0->62.5fps
 *
 */
void cap_framerate(double frameStart)
{
  double frameTime = SDL_GetTicks64() - frameStart;

  // 1000 / 60 = 16.666 ms per frame
  double target = 1000.0 / FPSCAP;

  // delay = 16.666 - (GetTicks - currentTime)
  double delay = target - frameTime;

  // 1000 / 16.666 = 60.0
  // 1000 / 16     = 62.5
  if (delay > 0)
  {
    SDL_Delay(delay);
  }
}

void loopdyLoop(IRenderer &ren, Mouse &mouse, Menu &mainMenu, SDL_Event &e){
  frameCount++;
  double currentTime = SDL_GetTicks64(); // current loop iteration time (ms)
  
  //Calculate deltatime ie.tickrate for animations:
  //ie. how long it took for last loop iter to reach this iter
  double deltaTime = ( currentTime - lastDeltaTime) / 1000.0;

  lastDeltaTime = currentTime;

  while (SDL_PollEvent(&e)){
      ListenGlobalKeys(ren,mouse,e);
  }

    /* @brief Frame sequence pipeline
   * @comment In SDL-style double buffering the canonical frame sequence is:
   * 1. Handle input 2. Update gamestate (physics, collisions, etc.)
   * 3. Clear render target(s) 4. Draw everything 5. Present (swap buffers)
   * @warning The order "UpdateCollisons->Clear->Update->Render" only shows the recently Cleared buffer.
   */

  calc_framerate(lastFpsTime,frameCount,gApp);

  //Update gamestates
  switch(gApp.mode)
  {
    case AppState::MAIN_MENU:
      UpdateCollisions(mouse);
      ListenGameKeys(player,player2); //use sdl_getkeyboardstate
      player.Update(deltaTime,"p1");
      player2.Update(deltaTime,"p2");
      //DEBUG RECT:
      //std::cout << "Racer pos: " << player2.srcRect.x << ", " << player2.srcRect.y << std::endl;
      break;
    
    case AppState::MINIGAME:
      UpdateCollisions(mouse);
      ListenGameKeys(player,player2); //use sdl_getkeyboardstate
      player.Update(deltaTime,"p1");
      player2.Update(deltaTime,"p2");
      break;

    case AppState::EXIT:
      break;
    default: throw std::invalid_argument("Undefined appstate from loopdyloop gamestates!"); break;
  }

  //Clear
  ren.Clear();

  //Render
  switch(gApp.mode)
  {
    case AppState::MAIN_MENU:
      ren.RenderMainMenu(mouse,mainmenuAssets);
      mainMenu.setBackground(*mainmenuAssets.spriteBg);
      mainMenu.setFrame(*mainmenuAssets.spriteFrame);
      mainMenu.setVisible(false);
      mainMenu.Render();
      break;
    
    case AppState::MINIGAME:
      ren.RenderMinigame(mouse,minigameAssets);
      break;

    case AppState::EXIT:
      break;
    default: throw std::invalid_argument("Undefined appstate from loopdyloop render!"); break;
  }

  //Present
  ren.Update();

  cap_framerate(currentTime);
}

int main (int argc, char *argv[])
{
  std::cerr << "FPSCAP: " << FPSCAP << std::endl;
  /*
  for (int i; i < argc; i++){
    if (strcmp(argv[1], "-sdl") == 0) {
        renderer = renderer_surface;
        break;
        }
    if (strcmp(argv[1], "-sdl") == 0) {
        renderer = renderer_texture;
        break;
        }
    if (strcmp(argv[1], "-opengl") == 0) {
        renderer = renderer_opengl;
        break;
        }
    if (strcmp(argv[1], "-vulkan") == 0) {
        renderer = renderer_vulkan;
        break;
        }
  }
  */

  std::cerr << "Initializing subsystems..." << std::endl;
  SurfaceRenderer ren;
  ren.initSubsystems(320,480);
  ren.setColors(gApp);

  std::cerr << "Initializing mixer..." << std::endl;
  Audio audio;
  audio.initMixer();

  std::cerr << "Initializing sprite callbacks..." << std::endl;
  setupSpriteCallbacks();

  std::cerr << "Loading media..." << std::endl;
  arial.Load("assets/arial.ttf", 24);

  std::cerr << "Setting up menus..." << std::endl;
  Menu mainMenu(ren,mouse,false);
  audio.playMusic();
  spritePause.Toggle();
  SDL_ShowCursor(SDL_DISABLE);

  gApp.mode = AppState::MAIN_MENU;
  SDL_Event e;
  while (gApp.mode != AppState::EXIT){
    loopdyLoop(ren, mouse, mainMenu, e);
  }

  arial.Shutdown();
  audio.Shutdown(bell, bgm);
  ren.Shutdown(gApp);
  return 0;
}
