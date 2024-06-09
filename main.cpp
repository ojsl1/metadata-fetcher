#include "render.h"
//#include "addons.h" //TODO
#include "mouse.h" //TODO

SDL_Window *window;
SDL_Surface *screen;
Mix_Chunk *bell;
Mix_Music *bgm;
double delta;

void cap_framerate ( Uint32 starting_tick ){
    if ( ( 1000 / fps ) > SDL_GetTicks() - starting_tick ){
      SDL_Delay( 1000 / fps - ( SDL_GetTicks() - starting_tick ) );
    }
};

void close ( SDL_Window* window, Mix_Chunk* bell, Mix_Music* bgm ){
    Mix_FreeChunk(bell);
    bell = NULL;
    Mix_FreeMusic(bgm);
    bgm = NULL;

    Mix_CloseAudio();
    SDL_FreeSurface(screen);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void prepClose ( SDL_Window* window, WindowDimensions dims ){
    //SDL_SetWindowSize( window, dims.wSize, dims.hSize ); //enforce size
    //SDL_SetWindowPosition( window, dims.xPosi, dims.yPosi ); //enforce position
    SDL_GetWindowPosition( window, &dims.xPosi, &dims.yPosi );
    std::cout << dims.xPosi << "," << dims.yPosi << std::endl;
    std::cout << dims.wSize << "," << dims.hSize << std::endl;
    std::cout << fps << std::endl;
}

int main( int argc, char *argv[] ){
    initVideo();
    initMixer();

    bool running = true;

    double delta = .0001;
    double time2 = SDL_GetTicks();
   
    Uint32 starting_tick;
    
    // TODO: below segfaults, add prior screen and screen->format debug checks here

    Uint32 beige = SDL_MapRGB( screen->format, 255, 255, 115 );
    Uint32 pink = SDL_MapRGB( screen->format, 232, 111, 148 );
    Uint32 blue = SDL_MapRGB( screen->format, 0, 0, 255 );
    Uint32 darkblue = SDL_MapRGB( screen->format, 111,114,120 );
    Uint32 darkgreen  = SDL_MapRGB( screen->format, 100,120,100 );

    // main loop
    while ( running ){
      delta = (SDL_GetTicks() - time2)/1000; //dividing with thousand to convert ms into s
      time2 = SDL_GetTicks(); // reset time2 for the next loop

      SDL_Event Event;
      Mouse mouse;

      starting_tick = SDL_GetTicks();
      cap_framerate( starting_tick );

      // event loop
      while ( SDL_PollEvent( &Event ) != 0 ){
          switch ( Event.type ){

            case SDL_QUIT:
              running = false;
              break;

            case SDL_MOUSEBUTTONDOWN: // event loop
              switch (Event.button.button){
                case SDL_BUTTON_LEFT:
                  break;
                case SDL_BUTTON_RIGHT:
                  break;
              }
              break;

            case SDL_MOUSEBUTTONUP: // event loop
              switch (Event.button.button){
                case SDL_BUTTON_LEFT:
                  break;
                case SDL_BUTTON_RIGHT:
                  break;
              }
              break;

            case SDL_KEYDOWN: // event loop
              switch (Event.key.keysym.sym){
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


      // fill screen with one color
      SDL_FillRect( screen, NULL, darkgreen );
      
      // Creating bare Sprites
      //Sprite object( red, window_width/2, window_height/2 );
      //Sprite another( blue, window_width/2-100, window_height/2+20 );
  
      // Creating Block sprites
      Block block1( pink, 120, 30 );
      Block block2( pink, 100, 15 );
      block1.set_image( "resources/cosmox2.png" );
      block2.set_image( "resources/avatar.bmp" );
  
      // Manipulate SpriteGroups
      SpriteGroup active_sprites;
      active_sprites.add( &block2 );
      active_sprites.add( &block1 );
  
      //active_sprites.add( &another );
      //object.draw( screen );
  
      //std::cout << active_sprites.has( another ) << std::endl;
  
      active_sprites.draw( screen );
  
      //SDL_UpdateWindowSurface( window );

      mouse.updateCursor();
      //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255)); // clear screen TODO: segfaults on t480s
      mouse.drawMouse( mouse.imageMouse );
      SDL_UpdateWindowSurface( window );


      } // end event loop
    } // end main loop

    WindowDimensions dims = {640, 480, 1920, 0};
    prepClose( window, dims);

    close(window, bell, bgm);

    return 0;
}
