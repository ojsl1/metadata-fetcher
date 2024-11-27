//wip separate handling

class RendererBase{
public:
    void initVideo(int window_width, int window_height);
    void initColors(SDL_Surface* gScreen);
    void clearScreen();
    void drawMainMenu();
    void drawAlleys();
    void present(); //for swapping buffers with opengl
    void cleanup(SDL_Window* gWindow);

private:
    SDL_Window* gWindow = NULL; // Renderer should manage this window
};

class Game{
public:
    void run(); // this function should include initSystems and the main loop inside, thus only requiring the function to be called from main.cpp

private:
    void initSystems(); // init subsystems like renderer, audio etc.
    void cap_framerate(Uint32 starting_tick);
    void cleanupSystems(WindowDimensions dims);
};

class Audio{
public:
    void initMixer();
    void cleanup(Mix_Chunk* bell, Mix_Music* bgm);
};
