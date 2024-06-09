#include "render.h"
//#include "addons.h" //TODO
#include "mouse.h" //TODO

SDL_Window *window;
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
             break;
            default: // event loop
              break;
          } // end outer switch

      mouse.updateCursor();
      mouse.drawMouse( mouse.imageMouse ); // TODO: segfaults on t480s
      SDL_UpdateWindowSurface( window ); // TODO: attempt to render every frame/tick

      } // end event loop
    } // end main loop

    WindowDimensions dims = {640, 480, 1920, 0};
    prepClose( window, dims);

    close(window, bell, bgm);

    return 0;
}
