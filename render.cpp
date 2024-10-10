#include "main.h" // globals
#include "render.h"

SDL_Window *gWindow = nullptr;
SDL_Surface *gScreen = nullptr;

void Renderer::initVideo( int window_width, int window_height ){
    SDL_Init( SDL_INIT_EVERYTHING );
    
    // Create the window
    gWindow = SDL_CreateWindow( "Cosmox's Playground",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width, // window_width
                                window_height, // window_height
                                SDL_WINDOW_RESIZABLE
                                //SDL_WINDOW_FULLSCREEN_DESKTOP
                                );

    if ( gWindow == NULL ){
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return;
    }

    //Note: this disables SDL_WINDOW_FULLSCREEN_DESKTOP
    SDL_SetWindowBordered(gWindow, SDL_TRUE);
    
    gScreen = SDL_GetWindowSurface(gWindow);
    if ( gScreen == NULL ){
      std::cerr << "SDL_ GetWindowSurface Error: " << SDL_GetError() << std::endl;
      SDL_DestroyWindow(gWindow);
      SDL_Quit();
      return;
    }
}

void Renderer::initMixer(){
    // MIX_DEFAULT_FREQUENCY = 22050
    Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 );
    Mix_Chunk *bell = NULL;
    Mix_Music *bgm = NULL;

    bell = Mix_LoadWAV( "sounds/school-bell.mp3" );
    bgm = Mix_LoadMUS( "sounds/sarajevo-pad-keys-melody_140bpm_C_minor.wav" );

    Mix_PlayChannel( -1, bell, 0);
    Mix_VolumeChunk( bell, 10 );
    SDL_Delay(1700);

    Mix_PlayMusic( bgm, -1 );
    Mix_VolumeMusic( 20 );
    //Mix_FadeInMusic( music1, 0, 5000 );
}

void Renderer::clearScreen(){
  // Clear the screen (optional, depends on game logic) with a custom uint32
  SDL_FillRect( gScreen, NULL, gPink );
  //SDL_FillRect(gScreen, NULL, SDL_MapRGB(gScreen->format, 255, 255, 255));
}

void Renderer::draw(){
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

}
