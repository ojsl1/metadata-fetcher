#include "render.h"
//#include "addons.h" //TODO

SDL_Window *window;
Mix_Chunk *bell;
Mix_Music *bgm;

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

    Uint32 starting_tick;

    SDL_Event event;
    bool running = true;
    while ( running ){
      starting_tick = SDL_GetTicks();
      cap_framerate( starting_tick );
      while ( SDL_PollEvent( &event ) != 0 ){
          switch ( event.type ){
              case SDL_QUIT:
                  running = false;
                  break;
              case SDLK_ESCAPE:
                  running = false;
                  break;
              /*case SDL_MOUSEBUTTONDOWN:
                  int x, y;
                  SDL_GetMouseState( &x, &y );

                  if ( isPointInRect( x, y, Button button1 ) ){
                      std::cout << "Button clicked!" << std::endl;
                  }
                  break;*/
              default:
                  break;
          }
      }
    }

    WindowDimensions dims = {640, 480, 1920, 0};
    prepClose( window, dims);

    close(window, bell, bgm);

    return 0;
}
