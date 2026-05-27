#ifndef RENDER_H
#define RENDER_H
#pragma once // temp fix pp_including_mainfile_in_preamble for input.h including this file

#include "main.h" // sdl

#include "sprite.h"     //Forward declare class for Render(...)
#include "input.h"      // ''
#include "font.h"       // ''
#include "character.h"  // ''

class Mouse;
class Font;
class Character;


/* TODO These structs shouldn't exist here */
struct MainMenuAssets {
    SurfaceSprite *spriteExit;
    SurfaceSprite *spriteTests;
    SurfaceSprite *spriteDrop;
    SurfaceSprite *spriteMute;
    SurfaceSprite *spritePause;
    SurfaceSprite *spriteBorder;
    SurfaceSprite *spriteFrame;
    SurfaceSprite *spriteBg;
    Font *arial;
    Character *player;
};
struct MinigameAssets {
    SurfaceSprite *spritePause;
    SurfaceSprite *spritePlaceholder;
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

    // @brief Draw functions.
    /* TODO Every draw call should _probably_ use DrawScaled */
    /* TODO Decouple the alternateSprite terniary check from Draw() */
    virtual void Draw(const SurfaceSprite& sprite, AppContext gApp) = 0;
    virtual void DrawScaled(const SurfaceSprite& sprite, AppContext gApp) = 0;

    // @brief Present the new frame.
    virtual void Update() = 0;

    // @brief Shutdown renderer and call SDL_QUIT
    /* TODO For renderer hotswapping this shouldn't call SDL_QUIT! */
    virtual void Shutdown(AppContext &gApp) = 0;

    /* TODO move these outta render */
    virtual void setColors(AppContext &gApp) = 0;
    /* TODO abstract these */
    virtual void RenderMainMenu( Mouse &mouse, const MainMenuAssets &assets ) = 0;
    virtual void RenderMinigame( Mouse &mouse, const MinigameAssets &assets ) = 0;
    // virtual void drawTexture(...) = 0;
};


class SurfaceRenderer : public IRenderer {
  friend class SurfaceSprite;
public:
    // SDL_Surface blitting

    // @brief Initialize everything.
    // @details Use SDL_INIT_EVERYTHING()
    // @warning Set SDL_SetWindowBordered() which disables SDL_WINDOW_FULLSCREEN_DESKTOP
    void initSubsystems(int window_width,
                        int window_height) override;

    // @brief Clear the screen.
    // @details Use SDL_FillRect() to fill with hotpink (255,50,255)
    void Clear() override;

    // @brief Draw sprites
    // @details Use SDL_BlitSurface()
    void Draw(const SurfaceSprite &sprite, AppContext ctx) override;

    // @brief Draw scaled sprites.
    // @details Use SDL_BlitScaled()->SDL_BlitSurface()
    void DrawScaled(const SurfaceSprite &sprite, AppContext ctx) override;

    // @brief Present the new frame.
    // @details Use SDL_UpdateWindowSurface()
    void Update() override;

    // @brief Shutdown renderer
    void Shutdown(AppContext &ctx) override;

    void setColors(AppContext &ctx) override;
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
