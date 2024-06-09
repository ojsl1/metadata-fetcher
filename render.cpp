#include "render.h"

void initVideo(){
    SDL_Init( SDL_INIT_EVERYTHING );
    
    // Create the window
    SDL_Window *window = NULL;
    window = SDL_CreateWindow( "イカは食用のいきものである",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width, // window_width
                                window_height, // window_height
                                SDL_WINDOW_RESIZABLE
                                //SDL_WINDOW_FULLSCREEN_DESKTOP
                                );

    if ( window == NULL ){
      std::cout << "window still null, \n" << SDL_GetError() << std::endl;
    }

    //Note: this disables SDL_WINDOW_FULLSCREEN_DESKTOP
    SDL_SetWindowBordered( window, SDL_TRUE );

    // Create a surface for the window
    SDL_Surface *screen = NULL;
    screen = SDL_GetWindowSurface( window );
}

void initMixer(){
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
    Mix_VolumeMusic( 10 );
    //Mix_FadeInMusic( music1, 0, 5000 );
}
