#ifndef ADDONS_H
#define ADDONS_H

#include "render.h"

void cap_framerate ( Uint32 starting_tick );
//void initColors(SDL_Surface* gScreen);
void printVectorTodos();

class Timer{
public:
  Timer() {
    lastTime = 0;
    currentTime = 0;
    deltaTime = 0.0f;
  }

  void Start(){
    lastTime = SDL_GetTicks(); // get the time when the timer starts
  }

  void Update(){
    currentTime = SDL_GetTicks(); // get the current time in milliseconds
    deltaTime = (currentTime - lastTime) / 1000.0f; // calculate delta time in seconds
    lastTime = currentTime; // update last time for the next frame
  }

  float getDeltaTime() const {
    return deltaTime;
  }

private:
  Uint32 lastTime; // time at the end of the last frame
  Uint32 currentTime; // time at the start of the current frame
  float deltaTime; // time difference between frames
};

#endif // ADDONS_H
