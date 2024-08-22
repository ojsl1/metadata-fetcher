#include "main.h" // globals
#include "input.h"
#include "button.h"

#define window_width 480
#define window_height 320
#define fps 15

enum AppState {
    MENU_STATE,
    PLAY_STATE,
    GALLERY_STATE,
};

Uint32 gPink;
Uint32 gRed;
Uint32 gBeige;
Uint32 gBlue;
Uint32 gDarkblue;
Uint32 gDarkgreen;
Mix_Chunk *bell;
Mix_Music *bgm;
double delta;
AppState currentState;

void cap_framerate ( Uint32 starting_tick ){
    if ( ( 1000 / fps ) > SDL_GetTicks() - starting_tick ){
      SDL_Delay( 1000 / fps - ( SDL_GetTicks() - starting_tick ) );
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

void preCloseOperations ( SDL_Window* gWindow, WindowDimensions dims ){
    //SDL_SetWindowSize( gWindow, dims.wSize, dims.hSize ); //enforce size
    //SDL_SetWindowPosition( gWindow, dims.xPosi, dims.yPosi ); //enforce position
    SDL_GetWindowPosition( gWindow, &dims.xPosi, &dims.yPosi );
    std::cout << "Exit Position: " << dims.xPosi << "," << dims.yPosi << std::endl;
    std::cout << "Exit Size: " << dims.wSize << "," << dims.hSize << " [TODO: Doesnt update after resizing]" << std::endl;
    std::cout << "FPS: " << fps << std::endl;
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

void printTodos(){
    std::cout << "########################## TO DO LIST ##########################" << std::endl;
    std::cout << "# HIGH PRIO: Rewrite the eventloop as a separate EventListener (mouse events) -> clickable buttons -> main menu -> submenus" << std::endl;
    std::cout << "# MED  PRIO: Separate classes from `render.h` to their own header files" << std::endl;
    std::cout << "# MED  PRIO: Is it possible to draw mouse using Sprite, SpriteGroup or Block classes" << std::endl;
    std::cout << "# MED  PRIO: was adding window_width and window_height return types to initVideo() really necessary when you made it a global/extern function" << std::endl;
    std::cout << "# LOW PRIO: Add listeners for ctrl+q and ctrl+w and q to exit the game" << std::endl;
    std::cout << "# NO  PRIO: test if `double delta` timer from main.cpp can be combined with cap_framerate()" << std::endl;
    std::cout << "# NO  PRIO: test if the SDL_Rect movable; from main.cpp works at all" << std::endl;
    std::cout << "# NO  PRIO: Currently all header files that require SDL libs source them through `render.h` -> `main.h`. \n Delete this superfluous step and directly source the SDL libs through main.h.\n For the affected header files this will require adding includes for the standard libs that are also in render.h (as of now just iostream and vector)." << std::endl;
    std::cout << "# NO  PRIO: implement gl_renderer.cpp (and vk_renderer.cpp) for the SDL_Render stuff and comments" << std::endl;
    std::cout << "# NO  PRIO: physics.h: finish the empty classes" << std::endl;
    std::cout << "# NO  PRIO: physics.h: vect2 to move sprites" << std::endl;
    std::cout << "# NO  PRIO: Refactor everything into OOP, with base classes and subclasses" << std::endl;
    std::cout << "# NO  PRIO: button.cpp: implement spritesheet capabilities" << std::endl;

    std::cout << "# WORK PRIO: autosort priorities on std::cout, so no need to rewrite the " << std::endl;
    std::cout << "# WORK PRIO: main.cpp: refactor game and event loops to include the enum game states" << std::endl;
    std::cout << "# WORK PRIO: button.cpp: change playButton sprite on interact without spritesheets" << std::endl;
}

#if 1
void printTodosNEW(){
  std::cout << "TODO: autosorting to-dos..." << std::endl;
  std::cout << "1. Create an int with switchcases for values 0-5. \n 2. On the list above append the int with prio value to each todo" << std::endl;
  std::cout << " 3. 1 current issues, 2 high issues, 3 med issues, 4 low issues 5 future issues" << std::endl;
  std::cout << " 4. main problems are what should the int be, class, func, enum, or what?" << std::endl;
  std::cout << " 5. and where would it be created at?" << std::endl;
}
#endif

int main( int argc, char *argv[] ){
    printTodos();
    printTodosNEW();

    initVideo(window_width, window_height);
    initColors(gScreen);
    initMixer();

    bool running = true;

    //double delta = .0001;
    //double time2 = SDL_GetTicks();
   
    Uint32 starting_tick;
    Button playButton(20,230);

    // main loop
    while ( running ){
      //delta = (SDL_GetTicks() - time2)/1000; //dividing with thousand to convert ms into s
      //time2 = SDL_GetTicks(); // reset time2 for the next loop

      SDL_Event e;
      Mouse mouse;

      starting_tick = SDL_GetTicks();
      cap_framerate( starting_tick );

      // event loop
      while ( SDL_PollEvent( &e ) != 0 ){
          switch ( e.type ){

            case SDL_QUIT:
              running = false;
              break;

            case SDL_MOUSEBUTTONDOWN:
              switch (e.button.button){
                case SDL_BUTTON_LEFT: {
                  break;
                }
                case SDL_BUTTON_RIGHT:
                  break;
              }
              break;

            case SDL_MOUSEBUTTONUP:
              switch (e.button.button){
                case SDL_BUTTON_LEFT:
                  if (playButton.selected){
                  std::cout << "play clicked with m1" << std::endl;
                  
                  }
                  break;
                case SDL_BUTTON_RIGHT:
                  if (playButton.selected){
                  std::cout << "play clicked with m2" << std::endl;
                  }
                  break;
              }
              break;

            case SDL_KEYDOWN: // event loop
              switch (e.key.keysym.sym){
                case SDLK_ESCAPE:
                  running = false;
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

            default: // event loop defult break
              break;
          } // end outer switch

      // 1. Update cursor position
      mouse.updateCursor();

      // 2. Check cursor interaction with buttons
      playButton.detectCursor(mouse);

      // 3. Clear the screen (optional, depends on game logic) with a custom uint32
      SDL_FillRect( gScreen, NULL, gPink );
      //SDL_FillRect(gScreen, NULL, SDL_MapRGB(gScreen->format, 255, 255, 255));

      // 4. Draw other elements (buttons, etc.)
      //
      // ################################
      // ##    Drawing operations      ##
      // ################################
      
      // Ex. Create Sprites
      Sprite object( gRed, window_width/2, window_height/2 );
      Sprite another( gBlue, window_width/2-100, window_height/2+20 );
  
      // Ex. Create Block sprites
      Block block1( gPink, 120, 30 );
      Block block2( gPink, 100, 15 );
      block1.set_image( "resources/cosmox2.png" );
      block2.set_image( "resources/avatar.bmp" );

      /* !TODO: Trying to blit cursor along with other sprites
      Block block3mouse( blue, 130, 40 );
      block3mouse.set_image( "resources/mouse.png" );
      SpriteGroup pointers;
      pointers.add( &block3mouse );
      pointers.draw( gScreen ); // draw to mouse not to screen
      */


      // Ex. Create a SpriteGroup & add previously created Blocks to it
      SpriteGroup active_sprites;
      active_sprites.add( &block2 );
      active_sprites.add( &block1 );
      active_sprites.add( &another );
      active_sprites.add( &object );
      object.draw( gScreen );
 
      // Ex. Test if SpriteGroup contains a specified sprite_object
      //std::cout << active_sprites.has( another ) << std::endl; // TODO: Print only once instead of spamming
  
      // Ex. Blit the whole SpriteGroup
      active_sprites.draw(gScreen);

      playButton.drawButton(gScreen);

      // 5. Draw the cursor on top of everything else
      mouse.drawCursor(gScreen);
      
      // 6. Update the window surface
      SDL_UpdateWindowSurface(gWindow);

      } // end event loop
    } // end main loop

    WindowDimensions dims = {640, 480, 1920, 0};
    preCloseOperations( gWindow, dims);
    closeSDL(gWindow, bell, bgm);
    return 0;
}
