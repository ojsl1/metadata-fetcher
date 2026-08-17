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

class IRenderer {
public:
    virtual ~IRenderer() = default;

    // @brief Initialize everything.
    virtual void init(int window_width, int window_height) = 0;

    // @brief Clear the screen.
    virtual void Clear() = 0;

    // @brief Draw functions.
    /* TODO Every draw call should _probably_ use DrawScaled */
    /* TODO Decouple the alternateSprite terniary check from Draw() */
    virtual void Draw(const SurfaceSprite& sprite, const AppContext &gApp) = 0;
    virtual void DrawScaled(const SurfaceSprite& sprite, const AppContext &gApp) = 0;

    // @brief Present the new frame.
    virtual void Update() = 0;

    // @brief Font stuff
    virtual void DrawText(const AppContext &app, int x, int y, const std::string &message) const = 0;
    virtual void DrawText(const AppContext &app, int x, int y, const std::string &message, SDL_Color color) const = 0;
    virtual void DrawText(const AppContext &app, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color) const = 0;
    virtual bool LoadFont(const char *filePath, int fontSize) = 0;

    // @brief Shutdown renderer and call SDL_QUIT
    virtual void Shutdown(AppContext &gApp) = 0;

    /* TODO move these outta render */
    virtual void setColors(AppContext &gApp) = 0;
    // virtual void drawTexture(...) = 0;
};


class SurfaceRenderer : public IRenderer {
  friend class SurfaceSprite;
public:
    // SDL_Surface blitting
    SurfaceRenderer();
    ~SurfaceRenderer();

    // @brief Initialize everything.
    // @details Use SDL_INIT_EVERYTHING()
    // @warning Set SDL_SetWindowBordered() which disables SDL_WINDOW_FULLSCREEN_DESKTOP
    void init(int window_width,
                        int window_height) override;

    // @brief Clear the screen.
    // @details Use SDL_FillRect() to fill with hotpink (255,50,255)
    void Clear() override;

    // @brief Draw static sprites
    // @details Use SDL_BlitSurface()
    void Draw(const SurfaceSprite &sprite, const AppContext &ctx) override;

    // @brief Draw animated sprites
    // @details takes in spritesheet as surface and draws the moving srcRect, update using Character::Update
    void Draw(SDL_Surface *surface, const SDL_Rect *srcRect, const SDL_Rect &dstRect, const AppContext &ctx);

    // @brief Draw scaled sprites.
    // @details Use SDL_BlitScaled()->SDL_BlitSurface()
    void DrawScaled(const SurfaceSprite &sprite, const AppContext &ctx) override;

    // @brief Present the new frame.
    // @details Use SDL_UpdateWindowSurface()
    void Update() override;

    // @brief Shutdown renderer
    void Shutdown(AppContext &ctx) override;

    // @brief Font stuff
    void DrawText(const AppContext &app, int x, int y, const std::string &message) const override;
    void DrawText(const AppContext &app, int x, int y, const std::string &message, SDL_Color color) const override;
    void DrawText(const AppContext &app, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color) const override;
    bool LoadFont(const char *filePath, int fontSize) override;

    void setColors(AppContext &ctx) override;

private:
    TTF_Font *font_;
};

class TextureRenderer : public IRenderer {
public:
    // SDL_Renderer / SDL_Texture
    //TODO stub
};

class GLRenderer : public IRenderer {
public:
    // OpenGL
    bool initGL();

    void init(int window_width, int window_height) override;

    void SwapBuffers(AppContext &gApp);
    void Update() override;
    void Clear() override;

    void Draw(const SurfaceSprite &sprite, const AppContext &gApp) override;
    void DrawScaled(const SurfaceSprite &sprite, const AppContext &gApp) override;

    void Shutdown(AppContext &ctx) override;

    void DrawText(const AppContext &app, int x, int y, const std::string &message) const override;
    void DrawText(const AppContext &app, int x, int y, const std::string &message, SDL_Color color) const override;
    void DrawText(const AppContext &app, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color) const override;
    bool LoadFont(const char *filePath, int fontSize) override;

    void setColors(AppContext &ctx) override;

protected:
    SDL_GLContext glContext;
};

class VkRenderer : public IRenderer {
public:
    // Vulkan
    //TODO stub
};

#endif // RENDER_H
