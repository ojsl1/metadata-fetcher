#include "main.h" // sdl
#include <GL/glew.h>
#include "render.h"
#include "sprite.h"
#include "util.h"
#include <iostream>

void SurfaceRenderer::setColors(AppContext &ctx)
{
    ctx.pink = SDL_MapRGB(ctx.screen->format, 232, 111, 148);
    ctx.red = SDL_MapRGB(ctx.screen->format, 250, 0, 0);
    ctx.beige = SDL_MapRGB(ctx.screen->format, 255, 255, 115);
    ctx.blue = SDL_MapRGB(ctx.screen->format, 0, 0, 255);
    ctx.darkblue = SDL_MapRGB(ctx.screen->format, 111, 114, 120);
    ctx.darkgreen = SDL_MapRGB(ctx.screen->format, 100, 120, 100);
}

void GLRenderer::setColors(AppContext &ctx) {
  //
}

bool GLRenderer::initGL() {
  //Success flag
  bool success = true;

  //Generate program
  gApp.gProgramID = glCreateProgram();

  //Create Vertex shader
  GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

  //Get Vertex source
  const GLchar *vertexShaderSource[] =
  {
    "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
  };

  //Set Vertex source
  glShaderSource( vertexShader, 1, vertexShaderSource, nullptr );

  //Compile Vertex source
  glCompileShader ( vertexShader );

  //Check vertex shader for errors
  GLint vShaderCompiled = GL_FALSE;
  glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
  if( vShaderCompiled != GL_TRUE )
  {
    std::cout << "Unable to compile vertex shader %d!" << vertexShader << std::endl;
    printShaderLog( vertexShader );
    success = false;
  }
  else
  {
    //Attach vertex shader to program
    glAttachShader( gApp.gProgramID, vertexShader );

    //Create fragment shader
    GLuint fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER );

    //Get fragment source
    const GLchar *fragmentShaderSource[] =
    {
      "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
    };

    //Set fragment source
    glShaderSource( fragmentShader, 1, fragmentShaderSource, nullptr );

    //Compile fragment source
    glCompileShader ( fragmentShader );

    //Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
    if( fShaderCompiled != GL_TRUE )
    {
      std::cout << "Unable to compile fragment shader %d!" << fragmentShader << std::endl;
      printShaderLog( fragmentShader );
      success = false;
    }
    else
    {
      //Attach fragment shader to program
      glAttachShader( gApp.gProgramID, fragmentShader );

      //Link program
      glLinkProgram( gApp.gProgramID );

      //Check for errors
      GLint programSuccess = GL_TRUE;
      glGetProgramiv( gApp.gProgramID, GL_LINK_STATUS, &programSuccess );
      if( programSuccess != GL_TRUE )
      {
        std::cout << "Error linking program: " << gApp.gProgramID << std::endl;
        printProgramLog( gApp.gProgramID );
        success = false;
      }
      else
      {
        //Get vertex attribute location
        gApp.gVertexPos2DLocation = glGetAttribLocation( gApp.gProgramID, "LVertexPos2D" );
        if( gApp.gVertexPos2DLocation == -1 )
        {
          std::cout << "LVertexPos2D is not a valid glsl program variable!" << std::endl;
          success = false;
        }
        else
        {
          //Initialize clear color
          glClearColor( 0.f, 0.f, 0.f, 1.f );

          //VBO data
          GLfloat vertexData[] =
          {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
          };

          //IBO data
          GLuint indexData[] = { 0, 1, 2, 3 };

          //Create VBO
          glGenBuffers( 1, &gApp.gVBO );
          glBindBuffer( GL_ARRAY_BUFFER, gApp.gVBO );
          glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

          //Create IBO
          glGenBuffers( 1, &gApp.gIBO );
          glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gApp.gIBO );
          glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
        }
      }
    }
  }
  return success;
}

void GLRenderer::init( int window_width, int window_height ) {
  //Use OpenGL 3.1 core (according to lazyfoo this version gets rid of "old" functionality)
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0){
      std::cerr << "Video init failed: " << SDL_GetError() << std::endl;
      return;
  }

  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0){
    std::cerr << "Audio init failed proceeding without audio: " << SDL_GetError() << std::endl;
  }

  if (TTF_Init() != 0){
    std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    return;
  }

  gApp.win.reset(SDL_CreateWindow("Metadata fetcher (GLRenderer)",
                                  0, 0, window_width, window_height,
                                  SDL_WINDOW_HIDDEN|SDL_WINDOW_OPENGL
  ));

  if ( gApp.win == nullptr ){
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return;
  }

  glContext = SDL_GL_CreateContext(gApp.win.get());
  if (glContext == nullptr)
  {
    std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return;
  }
  else
  {
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if ( glewError != GLEW_OK )
    {
      std::cerr << "glewInit Error: " << glewGetErrorString( glewError ) << std::endl;
    }

    //Use VSYNC
    if ( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
      std::cerr << "Warning: Unable to set VSYNC" << SDL_GetError() << std::endl;
    }

    //Initialize OpenGL
    if (!initGL())
    {
      std::cerr << "initGL error: " << SDL_GetError() << std::endl;
      return;
    }
  }
}

void GLRenderer::SwapBuffers(AppContext &gApp) {
  SDL_GL_SwapWindow(gApp.win.get());
}

void GLRenderer::Draw(const SurfaceSprite &sprite, const AppContext &ctx) {
  //TODO stub
}

void GLRenderer::DrawScaled(const SurfaceSprite &sprite, const AppContext &ctx) {
  //TODO stub
}

void GLRenderer::DrawText(const AppContext &, int, int, const std::string &) const
{
  //TODO stub
}

void GLRenderer::DrawText(const AppContext &, int, int, const std::string &, SDL_Color) const
{
  //TODO stub
}

void GLRenderer::DrawText(const AppContext &, int, int, const std::string &, SDL_Rect, SDL_Color) const
{
  //TODO stub
}

bool GLRenderer::LoadFont(const char *filePath, int fontSize) {
  //TODO stub
}

void GLRenderer::Update() {
  //TODO stub
}

void GLRenderer::Clear() {
  //TODO stub
}

void GLRenderer::Shutdown(AppContext &ctx)
{
  SDL_GetWindowPosition( ctx.win.get(), &ctx.window.x, &ctx.window.y );
  std::cout << "Exit Position: " << ctx.window.x << "," << ctx.window.y << std::endl;
  std::cout << "Exit Size: " << ctx.window.w << "," << ctx.window.h << std::endl;

  //Deallocate program
  glDeleteProgram( gApp.gProgramID );

  //Destroy window
  ctx.win.reset();

  //Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();

  std::cout << "Exit successfully" << std::endl;
}

SurfaceRenderer::SurfaceRenderer()
  : font_(nullptr)
{
  //
}

SurfaceRenderer::~SurfaceRenderer()
{
  if(font_){
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
}

void SurfaceRenderer::init( int window_width, int window_height )
{
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0){
    std::cerr << "Video init failed: " << SDL_GetError() << std::endl;
    return;
  }

  if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0){
    std::cerr << "Audio init failed proceeding without audio: " << SDL_GetError() << std::endl;
  }

  /*
  if (IMG_Init(IMG_INIT_PNG) != 0){
    std::cerr << "IMT_Init failed: " << IMG_GetError() << std::endl;
    return;
  }
  */

  if (TTF_Init() != 0){
    std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
    return;
  }

  gApp.win.reset(SDL_CreateWindow( "Metadata fetcher (SurfaceRenderer)",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              window_width, window_height,
                              SDL_WINDOW_RESIZABLE
                              //SDL_WINDOW_FULLSCREEN_DESKTOP
  ));

  if ( gApp.win == nullptr ){
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return;
  }

  SDL_SetWindowBordered(gApp.win.get(), SDL_TRUE);

  gApp.screen = SDL_GetWindowSurface(gApp.win.get());
  if ( gApp.screen == nullptr ){
    std::cerr << "SDL_GetWindowSurface Error: " << SDL_GetError() << std::endl;
    gApp.win.reset();
    SDL_Quit();
    return;
  }
}

void SurfaceRenderer::Clear()
{
  if (gApp.screen) {
    SDL_FillRect(gApp.screen, nullptr, SDL_MapRGB(gApp.screen->format, 255, 50, 255));
  }else {
    std::cout << "gApp.screen invalid during Clear, unable to clear." << std::endl;
  }
}

void SurfaceRenderer::Draw(SDL_Surface *surface,
                            const SDL_Rect *srcRect,
                            const SDL_Rect &dstRect,
                            const AppContext &ctx)
{
  if (!surface || !ctx.screen) return;

  // SDL_BlitSurface mutates the destination rect, so use a local copy.
  SDL_Rect dstRectLocal = dstRect;
  SDL_BlitSurface(surface, srcRect, ctx.screen, &dstRectLocal);
}

void SurfaceRenderer::Draw(const SurfaceSprite &sprite, const AppContext &ctx)
{
  Draw(sprite.GetDrawSurface(), sprite.GetSourceRect(), sprite.dRectSprite, ctx);
}

void SurfaceRenderer::DrawScaled(const SurfaceSprite &sprite, const AppContext &ctx){
  SDL_Surface *currentSprite = sprite.GetDrawSurface();
  const SDL_Rect *srcRect = sprite.GetSourceRect();

  if (!currentSprite || !ctx.screen){
      SDL_Log("Invalid input surface (draw surface or screen is null)\n");
      return;
  }

  #if DEBUG
  // Log and check draw surface bpp.
  SDL_Log("draw surface BitsPerPixel: %d\n", currentSprite->format->BitsPerPixel);
  if (currentSprite->format->BitsPerPixel != 32 &&
      currentSprite->format->BitsPerPixel != 16 &&
      currentSprite->format->BitsPerPixel != 8) {
      SDL_Log("Unsupported draw surface format: %d bpp\n", currentSprite->format->BitsPerPixel);
      return;
  }
  // Log draw surface pixel format details.
  SDL_Log("draw surface Pixel Format: Rmask: 0x%X, Gmask: 0x%X, Bmask: 0x%X, Amask: 0x%X\n",
          currentSprite->format->Rmask,
          currentSprite->format->Gmask,
          currentSprite->format->Bmask,
          currentSprite->format->Amask);
  #endif

  // Create a 32bpp scaled surface
  SDL_Surface *scaledSpriteLocal = SDL_CreateRGBSurface(
      currentSprite->flags,
      sprite.dRectSprite.w,
      sprite.dRectSprite.h,
      32,
      currentSprite->format->Rmask,
      currentSprite->format->Gmask,
      currentSprite->format->Bmask,
      currentSprite->format->Amask);

  if (!scaledSpriteLocal){
    SDL_Log("SDL_CreateRGBSurface failed: %s\n", SDL_GetError()); // e.g. "Unknown pixel format"
    return;
  }

  /**
   * @brief Create a local destination rectangle for custom scaling
   * @param Replace the fourth arg of SDL_BlitScaled with &scaleRect
   * @details If you want to scale only a portion of the rawSprite or
   * scale it to a specific region of the destination surface, e.g. if
   * scaleRect.w and scaleRect.h differ from scaledSprite.
   * @notes If the entire rawSprite is scaled to the exact dimensions of
   * scaledSpriteLocal, you don't need scaleRect. SDL_BlitScaled automatically matches
   * the size of the source surface to the destination surface if NULL is passed.
   */
   //SDL_Rect scaleRect = {0, 0, dRectSprite.w, dRectSprite.h}; // x,y,w,h

  // Scale the selected source onto scaledSpriteLocal.
  if (SDL_BlitScaled(currentSprite, srcRect, scaledSpriteLocal, nullptr) < 0){
      // NB: scaledSpriteLocal is 32bpp, the images have to also be 32bpp.
      SDL_Log("SDL_BlitScaled failed: %s\n", SDL_GetError()); // e.g. "Blit combination not supported"
      SDL_FreeSurface(scaledSpriteLocal);
      return;
  }

  // const SDL_Rect is not allowed, create local:
  SDL_Rect dstRect = sprite.dRectSprite;

  // Blit scaledSpriteLocal onto the screen.
  if (SDL_BlitSurface(scaledSpriteLocal, nullptr, ctx.screen, &dstRect) < 0){
      std::cout << "draw surface BitsPerPixel: " << currentSprite->format->BitsPerPixel << std::endl;
      std::cout << "scaledSprite BitsPerPixel: " << scaledSpriteLocal->format->BitsPerPixel << std::endl;
      SDL_Log("SDL_BlitSurface failed: %s\n", SDL_GetError());
  }

  // Free the local surface
  SDL_FreeSurface(scaledSpriteLocal);
}

// Overload #1 with default color
void SurfaceRenderer::DrawText(const AppContext &app,
                int x, int y,
                const std::string &message) const
{
  SDL_Color color = {0,0,0,255};
  SDL_Rect dstRect = {x,y,0,0};
  DrawText(app, x, y, message, dstRect, color);
}

// Overload #2 with custom color
void SurfaceRenderer::DrawText(const AppContext &app,
                int x, int y,
                const std::string &message,
                SDL_Color color) const
{
  SDL_Rect dstRect = {x,y,0,0};
  DrawText(app, x, y, message, dstRect, color);
}

// Overload #3 with bounding box and alignment
void SurfaceRenderer::DrawText(const AppContext &app,
                int x, int y,
                const std::string &message,
                SDL_Rect dstRect,
                SDL_Color color) const
{
  if(!font_){
    std::cerr << "No font loaded, cannot render text!.\n";
    return;
  }

  //Alternatively: for antialiased text use TTF_RenderText_Blended:
  //SDL_Surface *textSurface = TTF_RenderText_Blended(font_, message.c_str(), color_);

  SDL_Surface *textSurface = TTF_RenderText_Solid(font_, message.c_str(), color);
  if(!textSurface){
    std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
    return;
  }

  dstRect.x = x;
  dstRect.y = y;

  if (SDL_BlitSurface(textSurface, nullptr, app.screen, &dstRect) < 0){
    SDL_Log("SDL_BlitSurface failed: %s", SDL_GetError());
  }

  SDL_FreeSurface(textSurface);
}

bool SurfaceRenderer::LoadFont(const char *filePath, int fontSize){
  // Open the font style
  font_ = TTF_OpenFont(filePath, fontSize);
  if(!font_){
    std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
    Shutdown(gApp);
    return true;
  }
  return false;
}

void SurfaceRenderer::Update()
{
  SDL_UpdateWindowSurface(gApp.win.get());
}

void SurfaceRenderer::Shutdown(AppContext &ctx)
{
    //SDL_SetWindowPosition( gApp.windowHandle, gApp.window.x, gApp.window.y ); //enforce position
    //SDL_SetWindowSize( gApp.windowHandle, gApp.window.w, gApp.window.h ); //enforce size
    SDL_GetWindowPosition( ctx.win.get(), &ctx.window.x, &ctx.window.y );
    std::cout << "Exit Position: " << ctx.window.x << "," << ctx.window.y << std::endl;
    std::cout << "Exit Size: " << ctx.window.w << "," << ctx.window.h << " [FIXME: Doesnt update after resizing]" << std::endl;

    //Free fonts
    if(font_){
      TTF_CloseFont(font_);
      font_ = nullptr;
    }
    TTF_Quit();

    //Destroy window
    ctx.screen = nullptr;
    ctx.win.reset();

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

    std::cout << "Exit successfully" << std::endl;
}
