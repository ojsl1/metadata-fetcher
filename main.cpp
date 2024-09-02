#include "main.h" // globals
#include "input.h" // -> render.h -> <vector>
#include "button.h"

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 320
#define FPS 15

enum AppState {
    MENU_STATE,
    PLAY_STATE,
    GALLERY_STATE,
    EXIT_STATE,
};

// System declarations
AppState currentState;
double delta;
bool running;

// Debug declarations
WindowDimensions dims;

// Resource declarations
Mix_Chunk *bell;
Mix_Music *bgm;
Button playButton(20,230);

// WIP Color declarations; currently redundantly also in main.h as externs
Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;

void cap_framerate ( Uint32 starting_tick ){
    if ( ( 1000 / FPS ) > SDL_GetTicks() - starting_tick ){
      SDL_Delay( 1000 / FPS - ( SDL_GetTicks() - starting_tick ) );
    }
};

void closeSDL ( SDL_Window* gWindow, Mix_Chunk* bell, Mix_Music* bgm ){
    Mix_FreeChunk(bell);
    bell = NULL;
    Mix_FreeMusic(bgm);
    bgm = NULL;

    Mix_CloseAudio();
    SDL_FreeSurface(gScreen);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void handleExitState( SDL_Window* gWindow, WindowDimensions dims){
    //SDL_SetWindowSize( gWindow, dims.wSize, dims.hSize ); //enforce size
    //SDL_SetWindowPosition( gWindow, dims.xPosi, dims.yPosi ); //enforce position
    SDL_GetWindowPosition( gWindow, &dims.xPosi, &dims.yPosi );
    std::cout << "Exit Position: " << dims.xPosi << "," << dims.yPosi << std::endl;
    std::cout << "Exit Size: " << dims.wSize << "," << dims.hSize << " [TODO: Doesnt update after resizing]" << std::endl;
    std::cout << "FPS: " << FPS << std::endl;
    std::cout << "Exit succesfully" << std::endl;
}

void initColors(SDL_Surface* gScreen){
    gPink = SDL_MapRGB(gScreen->format, 232, 111, 148);
    gRed = SDL_MapRGB(gScreen->format, 250, 0, 0);
    gBeige = SDL_MapRGB(gScreen->format, 255, 255, 115);
    gBlue = SDL_MapRGB(gScreen->format, 0, 0, 255);
    gDarkblue = SDL_MapRGB(gScreen->format, 111, 114, 120);
    gDarkgreen = SDL_MapRGB(gScreen->format, 100, 120, 100);
}

void printVectorTodos(){
  //Store all to-dos in a vector of pairs (priority, description)
  // 1: current, 2: high, 3: medium, 4: low, 5: future
  std::vector<std::pair<int, std::string>> todos = {
    {1, "REFACTOR \n input.h: refactor mouse class according to the comments"},
    {1, "CLEANUP \n to avoid confusion make sure all variables are declared only either GLOBALLY or LOCALLY"},

    {2, "FEATURE \n implement a state-agnostic viewport render function and clean handleMenuState()"},
    {2, "FEATURE \n Add destructors to classes where appropriate"},
    {2, "FEATURE \n Make class members private where appropriate"},

    {3, "button.cpp: change playButton sprite on interact without spritesheets"},
    {6, "Rewrite the eventloop as a separate EventListener (mouse events) -> clickable buttons -> main menu -> submenus"},

    {6, "Separate classes from `render.h` to their own header files"},
    {6, "Is it possible to draw mouse using Sprite, SpriteGroup or Block classes"},
    {6, "was adding window_width and window_height return types to initVideo() really necessary when you made it a global/extern function"},

    {4, "BUG \n Fix clicking X not closing the game"},
    {4, "Add listeners for ctrl+q and ctrl+w and q to exit the game"},

    {6, "FEATURE \n test if `double delta` timer from main.cpp can be combined with cap_framerate()"},
    {6, "Currently all header files that require SDL libs source them through `render.h` -> `main.h`. \n Delete this superfluous step and directly source the SDL libs through main.h.\n For the affected header files this will require adding includes for the standard libs that are also in render.h (as of now just iostream and vector)."},
    {6, "FEATURE \n implement gl_renderer.cpp (and vk_renderer.cpp) for the SDL_Render stuff and comments"},
    {5, "physics.h: finish the empty classes"},
    {5, "FEATURE \n physics.h: vect2 to move sprites"},
    {6, "Refactor everything into OOP, with base classes and subclasses"},
    {5, "FEATURE \n button.cpp: implement spritesheet capabilities"},
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

void handleMenuState() {
  SDL_Event e;
  Mouse mouse; // cannot globally declare this because theres SDL_ShowCursor() inside that requires sdl to be initialized first
  while (SDL_PollEvent(&e)) {
    switch (e.type){
      //TODO case 1. if play rect pressed -> currentState = PLAY_STATE;
      //TODO case 2. if gallery rect pressed -> currentState = GALLERY_STATE;

      case SDL_MOUSEBUTTONUP:
        switch (e.button.button){
          case SDL_BUTTON_LEFT:
            std::cout << "value is: " <<  playButton.selected << std::endl;

            if (playButton.selected){
            std::cout << "Play Music... (m1)" << std::endl;
            }
            break;

          case SDL_BUTTON_RIGHT:
            std::cout << "value is: " <<  playButton.selected << std::endl;

            if (playButton.selected){
            std::cout << "Play Music... (m2)" << std::endl;
            }
            break;
        }
        break;

      case SDL_KEYDOWN: // event loop
        switch (e.key.keysym.sym){
          case SDLK_ESCAPE:
            currentState = EXIT_STATE;
            break;
          case SDLK_UP:
            std::cout << "up key works!" << std::endl;
            break;
          case SDLK_DOWN:
            std::cout << "down key works!" << std::endl;

          default:
            break;
        }
        break;
    }
  }

  // Update cursor position
  mouse.updateCursor();
  playButton.detectCursor(mouse);

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

  // Draw the play button
  playButton.drawButton(gScreen);

  // Draw the cursor on top of everything else
  mouse.drawCursor(gScreen);
  
  // Update the window surface
  SDL_UpdateWindowSurface(gWindow);
}

void handlePlayState() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "Entering Play State... TODO" << std::endl;
      //add playstate-specific input handling, probably none
      // case 2. "Returning to Main Menu"
  }
  //Playing music, change play rect into pause rect
}

void handleGalleryState() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    std::cout << "Entering Gallery State... TODO" << std::endl;

    //add gallery-specific input handling
    //if 2. Returning to Main Menu..." -> currentState = MENU_STATE;
  }
  // Render gallery state (ie. gallery viewport, images etc.)
}

int main (int argc, char *argv[]){
  printVectorTodos();
  initVideo(WINDOW_WIDTH, WINDOW_HEIGHT);
  initColors(gScreen);
  initMixer();
  bool running = true;
  currentState = MENU_STATE;
  //double delta = .0001;
  //double time2 = SDL_GetTicks();  
  Uint32 starting_tick;
  Mouse mouse; // cant have this globally declared because it has SDL stuff inside that need to be initialized first
  
  while (running){
    starting_tick = SDL_GetTicks();
    cap_framerate( starting_tick );

    switch (currentState){
      case EXIT_STATE:
        handleExitState(gWindow, dims);
        running = false;
        break;
      case MENU_STATE:
        handleMenuState();
        break;
      case PLAY_STATE:
        std::cout << "Entering Play State... TODO" << std::endl;
        handlePlayState();
        break;
      case GALLERY_STATE:
        std::cout << "Entering Gallery State... TODO" << std::endl;
        handleGalleryState();
        break;
      default:
        std::cout << "This shouldn't happen" << std::endl;
        break;
    }
  }
  
  // main function logic
}
