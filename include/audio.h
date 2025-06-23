#ifndef AUDIO_H
#define AUDIO_H

#include <../main.h>

class Audio{
public:
    void initMixer();
    void playMusic();
    void Shutdown(Mix_Chunk* bell, Mix_Music* bgm);
    Mix_Chunk *bell = NULL;
    Mix_Music *bgm = NULL;
};

inline void Audio::initMixer(){
    // MIX_DEFAULT_FREQUENCY = 22050
    Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 );

    bell = Mix_LoadWAV( "sounds/school-bell.mp3" );
    bgm = Mix_LoadMUS( "sounds/sarajevo-pad-keys-melody_140bpm_C_minor.wav" );

    Mix_PlayChannel( -1, bell, 0);
    Mix_VolumeChunk( bell, 10 );
    //SDL_Delay(1700);
}

inline void Audio::playMusic(){
    Mix_PlayMusic( bgm, -1 );
    Mix_VolumeMusic( 20 );
    //Mix_FadeInMusic( music1, 0, 5000 );
}

inline void Audio::Shutdown(Mix_Chunk* bell, Mix_Music* bgm){
    if (bell != nullptr) {
      Mix_FreeChunk(bell);
      bell = nullptr;
    }
    if (bgm != nullptr) {
      Mix_FreeMusic(bgm);
      bgm = nullptr;
    }
    Mix_CloseAudio();
}

#endif // AUDIO_H
