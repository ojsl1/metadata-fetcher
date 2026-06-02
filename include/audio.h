#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>

class Audio{
public:
    void initMixer();
    void playMusic() const;
    void Shutdown(Mix_Chunk* bell, Mix_Music* bgm);
    Mix_Chunk *bell = nullptr;
    Mix_Music *bgm = nullptr;
};

inline void Audio::initMixer(){
    // MIX_DEFAULT_FREQUENCY = 22050
    Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 );

    bell = Mix_LoadWAV( "sounds/school-bell.mp3" );
    bgm = Mix_LoadMUS( "sounds/sarajevo-pad-keys-melody_140bpm_C_minor.wav" );

    Mix_PlayChannel( -1, bell, 0);
    Mix_VolumeChunk( bell, 0 ); //
    //SDL_Delay(1700);
}

inline void Audio::playMusic() const
{
    Mix_PlayMusic( bgm, -1 );
    Mix_VolumeMusic( 30 );
    //Mix_FadeInMusic( music1, 0, 5000 );
}

inline void Audio::Shutdown(Mix_Chunk* bell, Mix_Music* bgm){
    if (bell != nullptr) {
      Mix_FreeChunk(bell);
    }
    if (bgm != nullptr) {
      Mix_FreeMusic(bgm);
    }
    Mix_CloseAudio();
}

#endif // AUDIO_H
