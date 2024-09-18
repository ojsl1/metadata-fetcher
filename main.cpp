#include "main.h" // globals
#include "input.h" // -> render.h -> <vector>
#include "button.h"
#include "addons.h"

enum AppState {
    MENU_STATE,
    PLAY_STATE,
    GALLERY_STATE,
    EXIT_STATE,
};

// System definitions
AppState currentState;
double delta;
bool running;

// Debug definitions
WindowDimensions dims;

// Resource definitions
Mix_Chunk *bell;
Mix_Music *bgm;
Button playButton(20, 230, 25, 25, "resources/button-inactive.png");
Button topleftButton(10, 10, 80, 80, "resources/play-pause.png");

// Color definitions
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

void loopMenuState() {
  SDL_Event e;
  Mouse mouse; // cannot globally declare this because theres SDL_ShowCursor() inside that requires sdl to be initialized first
  while (SDL_PollEvent(&e)) {
    switch (e.type){

      case SDL_MOUSEBUTTONUP:
        switch (e.button.button){

          case SDL_BUTTON_LEFT:{
            std::cout << "value is: " <<  playButton.selected << std::endl;
            if (playButton.selected){std::cout << "Play Music... (m1)" << std::endl;};
            if (topleftButton.selected){std::cout << "currentState = PLAY_STATE..." << std::endl;}
            } break;

          case SDL_BUTTON_RIGHT:{
            std::cout << "value is: " <<  playButton.selected << std::endl;
            if (playButton.selected){std::cout << "Play Music... (m2)" << std::endl;}
            } break;

        } break;

      case SDL_KEYDOWN:{
        switch (e.key.keysym.sym){
          case SDLK_ESCAPE: {currentState = EXIT_STATE;} break;
          case SDLK_UP: {std::cout << "up key works!" << std::endl;} break;
          case SDLK_DOWN: {std::cout << "down key works!" << std::endl;} break;
          default: break;}

        } break;

      default: break;}
  }

  // Update cursor position
  mouse.updateCursor();

  // Detect button-cursor collisions
  playButton.detectCursor(mouse);
  topleftButton.detectCursor(mouse);

  // Clear the screen (optional, depends on game logic) with a custom uint32
  SDL_FillRect( gScreen, NULL, gPink );
  //SDL_FillRect(gScreen, NULL, SDL_MapRGB(gScreen->format, 255, 255, 255));

  // ## Drawing operations ##
  // ALLEY. Create Sprites
  Sprite object( gRed, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 );
  Sprite another( gBlue, WINDOW_WIDTH/2-100, WINDOW_HEIGHT/2+20 );

  // ALLEY. Create Block sprites
  Block block1( gPink, 120, 30 );
  Block block2( gPink, 100, 15 );
  block1.set_image( "resources/cosmox2.png" );
  block2.set_image( "resources/avatar.bmp" );

  /*
  TODO: ALLEY. Trying to blit cursor along with other sprites
  Block block3mouse( blue, 130, 40 );
  block3mouse.set_image( "resources/mouse.png" );
  SpriteGroup pointers;
  pointers.add( &block3mouse );
  pointers.draw( gScreen ); // draw to mouse not to screen
  */

  // ALLEY. Create a SpriteGroup & add previously created Blocks to it
  SpriteGroup active_sprites;
  active_sprites.add( &block2 );
  active_sprites.add( &block1 );
  active_sprites.add( &another );
  active_sprites.add( &object );
  object.draw( gScreen );

  // ALLEY. Test if SpriteGroup contains a specified sprite_object
  //std::cout << active_sprites.has( another ) << std::endl; // TODO: Print only once instead of spamming

  // ALLEY. Blit the whole SpriteGroup
  active_sprites.draw(gScreen);

  // Draw the buttons
  playButton.drawButton(gScreen);
  topleftButton.drawButtonScaled(gScreen);

  // Draw the cursor last ie. on top of everything else
  mouse.drawCursor(gScreen);
  
  // Update the window surface ie. display new draw ops ie. new frame
  SDL_UpdateWindowSurface(gWindow);
}

void loopPlayState() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "WIP Clicked Play button..." << std::endl;
    std::cout << "WIP currentState = PLAY_STATE;" << std::endl;
    std::cout << "WIP Playing bgm music..." << std::endl;
    std::cout << "WIP Render Pause button into Play button rect..." << std::endl;
    std::cout << "WIP Clicked Pause button..." << std::endl;
    std::cout << "WIP Pause bgm music..." << std::endl;
    std::cout << "WIP currentState = MENU_STATE;" << std::endl;
  }
}

void loopGalleryState() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "TODO Entering Gallery State..." << std::endl;

    //add gallery-specific input handling
    //if 2. Returning to Main Menu..." -> currentState = MENU_STATE;
  }
  // Render gallery state (ie. gallery viewport, images etc.)
}

void loopExitState( SDL_Window* gWindow, Mix_Chunk* bell, Mix_Music* bgm, WindowDimensions dims){
    //SDL_SetWindowSize( gWindow, dims.wSize, dims.hSize ); //enforce size
    //SDL_SetWindowPosition( gWindow, dims.xPosi, dims.yPosi ); //enforce position
    SDL_GetWindowPosition( gWindow, &dims.xPosi, &dims.yPosi );
    std::cout << "Exit Position: " << dims.xPosi << "," << dims.yPosi << std::endl;
    std::cout << "Exit Size: " << dims.wSize << "," << dims.hSize << " [TODO: Doesnt update after resizing]" << std::endl;
    std::cout << "FPS: " << FPS << std::endl;

    Mix_FreeChunk(bell);
    bell = NULL;
    Mix_FreeMusic(bgm);
    bgm = NULL;

    Mix_CloseAudio();
    SDL_FreeSurface(gScreen);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    std::cout << "Exit succesfully" << std::endl;
}

int main (int argc, char *argv[]){
  printVectorTodos();
  initVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
  initColors(gScreen);
  initMixer();

  Timer timer;
  timer.Start();

  Uint32 starting_tick;

  bool running = true;
  currentState = MENU_STATE;
  Mouse mouse; // cant have this globally declared because it has SDL stuff inside that need to be initialized first
  
  while (running){
    starting_tick = SDL_GetTicks();
    cap_framerate( starting_tick );

    timer.Update();
    float deltaTime = timer.getDeltaTime();
    std::cout << "Delta time: " << deltaTime << " seconds" << std::endl;

    switch (currentState){
      case EXIT_STATE: {loopExitState(gWindow, bell, bgm, dims); running = false;} break;
      case MENU_STATE: {loopMenuState();} break;
      case PLAY_STATE: {std::cout << "Entering Play State... TODO" << std::endl; loopPlayState();} break;
      case GALLERY_STATE: {std::cout << "Entering Gallery State... TODO" << std::endl; loopGalleryState();} break;
      default: {std::cout << "This shouldn't happen" << std::endl;} break;
    }
  }
  
  // main function logic
}
