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

struct MainMenuAssets{
    Sprite &spriteExit;
    Sprite &spriteTests;
    Sprite &spriteDrop;
    Sprite &spriteMute;
    Sprite &spritePause;
    Sprite &spriteBorder;
    Sprite &spriteFrame;
    Sprite &spriteBg;
    Font &arial;
    Character &player;
};

struct MinigameAssets{
    Sprite& spritePause;
    Sprite& spritePlaceholder;
    Font& arial;
    Character &player2;
};

class RendererBase{
public:
   /* @brief Initialize everything.
    * @details Uses SDL_INIT_EVERYTHING()
    * @warning Sets SDL_SetWindowBordered() which disables SDL_WINDOW_FULLSCREEN_DESKTOP
    */
    void initVideo(int window_width, int window_height);
   /* @todo move this outta render.h
    */
    void initColors(AppContext gApp);
   /* @brief Clear the screen.
    * @details Uses SDL_FillRect() to fill with 255,50,255 ie. hotpink
    */
    void Clear();
    void RenderMainMenu( Mouse &mouse, const MainMenuAssets &assets );
    void RenderMinigame( Mouse &mouse, const MinigameAssets &assets );

   /* @brief Present the new frame.
    * @details Uses SDL_UpdateWindowSurface()
    * @deprecated should be refactored to use SDL_RenderPresent()
    */
    void Update();
    void Shutdown(AppContext gApp);
};

#endif // RENDER_H
