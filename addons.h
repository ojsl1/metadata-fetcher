#ifndef ADDONS_H
#define ADDONS_H

#include "render.h"

void cap_framerate ( Uint32 starting_tick );
void initColors(SDL_Surface* gScreen);
void printVectorTodos();

#if 0
#define FACTOR 7

class Addons{
  public:
      void skeletonVoid(); // skeleton void function
};

class Scaler{
  public:

      int scale(){
          return(FACTOR*ins);
      }
};
#endif

#endif // ADDONS_H
