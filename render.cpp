#include "render.h"

void initVideo(){
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window *window = NULL;

    window = SDL_CreateWindow( "イカは食用のいきものである",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                window_width, // window_width
                                window_height, // window_height
                                SDL_WINDOW_RESIZABLE
                                //SDL_WINDOW_FULLSCREEN_DESKTOP
                                );

    if ( window == NULL){
      std::cout << "Error initializing the window! "
        << std::endl
        << SDL_GetError()
        << std::endl;
    }
   
    //Note: below disables SDL_WINDOW_FULLSCREEN_DESKTOP
    SDL_SetWindowBordered( window, SDL_TRUE );

    //SDL_Surface *screen = SDL_GetWindowSurface( window );
    
    SDL_Surface *screen = NULL;
    screen = SDL_GetWindowSurface( window );

    //unsigned int 32
    Uint32 beige = SDL_MapRGB( screen->format, 255, 255, 115 );
    Uint32 pink   = SDL_MapRGB( screen->format, 232, 111, 148 );
    Uint32 blue  = SDL_MapRGB( screen->format, 0, 0, 255 );
    Uint32 darkblue  = SDL_MapRGB( screen->format, 111,114,120 );
    Uint32 darkgreen  = SDL_MapRGB( screen->format, 100,120,100 );
    SDL_FillRect( screen, NULL, darkgreen );

    //Sprite object( red, window_width/2, window_height/2 );
    //Sprite another( blue, window_width/2-100, window_height/2+20 );

    Block block1( pink, 120, 30 );
    Block block2( pink, 100, 15 );
    block1.set_image( "cosmox2.png" );
    block2.set_image( "avatar.bmp" );

    SpriteGroup active_sprites;
    active_sprites.add( &block2 );
    active_sprites.add( &block1 );
    //active_sprites.add( &another );
    //object.draw( screen );

    //std::cout << active_sprites.has( another ) << std::endl;

    active_sprites.draw( screen );

    SDL_UpdateWindowSurface( window );

    // Init mixer
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
