#ifndef RENDER_H
#define RENDER_H
#include "main.h" // globals

#if ALLEYS
#include <iostream>
#include <vector>
#endif

#include "input.h"  // forward declare Mouse class for Draw()
#include "sprite.h" // forward declare Sprite class for Draw()
#include "font.h"   // forward declare Font class for Draw()

#include <sstream>  //fpsText uses this

class Font;
class Mouse;
class Sprite;
class Character;

class RendererBase{
public:
    void initVideo(int window_width, int window_height);
    void initColors(SDL_Surface *gScreen);
    void Clear();
    void Render(Mouse &mouse, Sprite &spriteExit, Sprite &spriteTests,
         Sprite &spriteDrop, Sprite &spriteMute, Sprite &spritePause,
         Sprite &spriteBorder, Sprite &spriteFrame, Sprite &spriteBg,
         Font &arial, Character &player
         );
    void DrawTests();
    void Update();
    void Shutdown(SDL_Window *gWindow, WindowDimensions dims);
};

#endif // RENDER_H
