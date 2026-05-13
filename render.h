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

// TODO:
// Separate 1. draw calls (RenderMainMenu,RenderMinigame) and afterwards
// 2. asset structs (MainMenuAssets,MinigameAssets) into main.cpp/main.h

struct MainMenuAssets{
    Sprite *spriteExit;
    Sprite *spriteTests;
    Sprite *spriteDrop;
    Sprite *spriteMute;
    Sprite *spritePause;
    Sprite *spriteBorder;
    Sprite *spriteFrame;
    Sprite *spriteBg;
    Font *arial;
    Character *player;
};

struct MinigameAssets{
    Sprite *spritePause;
    Sprite *spritePlaceholder;
    Font *arial;
    Character *player2;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    // @brief Initialize everything.
    virtual void initSubsystems(int window_width, int window_height) = 0;

    // @brief Clear the screen.
    virtual void Clear() = 0;

    // @brief Present the new frame.
    virtual void Update() = 0;

    // @brief Shutdown renderer and call SDL_QUIT
    virtual void Shutdown(AppContext &gApp) = 0;

    /* TODO move these outta render */
    virtual void setColors(AppContext &gApp) = 0;
    virtual void RenderMainMenu( Mouse &mouse, const MainMenuAssets &assets ) = 0;
    virtual void RenderMinigame( Mouse &mouse, const MinigameAssets &assets ) = 0;
};


class SurfaceRenderer : public IRenderer {
public:
    // SDL_Surface blitting

    /* @brief Initialize everything.
     * @details Use SDL_INIT_EVERYTHING()
     * @warning Set SDL_SetWindowBordered() which disables SDL_WINDOW_FULLSCREEN_DESKTOP
     */
    void initSubsystems(int window_width,
                        int window_height) override;

    /* @brief Clear the screen.
     * @details Use SDL_FillRect() to fill with 255,50,255 ie. hotpink
     */
    void Clear() override;

    /* @brief Present the new frame.
     * @details Use SDL_UpdateWindowSurface()
     */
    void Update() override;

    // @brief Shutdown renderer and call SDL_QUIT
    void Shutdown(AppContext &gApp) override;

    /* @todo move these outta render.h */
    void setColors(AppContext &gApp) override;
    void RenderMainMenu( Mouse &mouse,
                         const MainMenuAssets &assets ) override;
    void RenderMinigame( Mouse &mouse,
                         const MinigameAssets &assets ) override;
};

class TextureRenderer : public IRenderer {
public:
    // SDL_Renderer / SDL_Texture
};

class GLRenderer : public IRenderer {
public:
    // OpenGL
};

class VkRenderer : public IRenderer {
public:
    // Vulkan
};

#endif // RENDER_H
