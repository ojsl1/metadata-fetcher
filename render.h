#ifndef RENDER_H
#define RENDER_H
#pragma once // temp fix pp_including_mainfile_in_preamble for input.h including this file

#include "main.h" // sdl

#include "sprite.h"     //Forward declare class for Render(...)
#include "input.h"      // ''
#include "font.h"       // ''
#include "character.h"  // ''

class Sprite;
class Mouse;
class Font;
class Character;

class RendererBase{
public:
    void initVideo(int window_width, int window_height);
    void initColors(AppContext gApp);
    void Clear();
    void Render(Mouse &mouse, Sprite &spriteExit, Sprite &spriteTests,
         Sprite &spriteDrop, Sprite &spriteMute, Sprite &spritePause,
         Sprite &spriteBorder, Sprite &spriteFrame, Sprite &spriteBg,
         Sprite &spritePlaceholder,
         Font &arial, Character &player, Character &player2
         );
    void Update();
    void Shutdown(AppContext gApp);
};

#endif // RENDER_H
