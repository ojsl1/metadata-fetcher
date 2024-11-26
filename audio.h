#ifndef AUDIO_H
#define AUDIO_H

class Audio{
public:
    void initMixer();
    void Shutdown(Mix_Chunk* bell, Mix_Music* bgm);
};

inline void Audio::initMixer(){
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

inline void Audio::Shutdown(Mix_chunk* bell, Mix_Music* bgm){
    if (bell != NULL) {
      Mix_FreeChunk(bell);
      bell = NULL;
    }
    if (bgm != NULL) {
      Mix_FreeMusic(bgm);
      bgm = NULL;
    }
    Mix_CloseAudio()
}

#endif // AUDIO_H
