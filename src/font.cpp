#include "font.h"
#include "main.h"

Font::Font()
  : font_(nullptr)
{
}

Font::~Font(){
  Free();
}

void Font::Free(){
  if(font_){
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
}

void Font::Shutdown(){
  if(font_){
    TTF_CloseFont(font_);
    font_ = nullptr;
  }
  TTF_Quit(); //TODO quit elsewhere, it doesnt belong here
}

bool Font::Load(const char *filePath, int fontSize){
  // Open the font style
  font_ = TTF_OpenFont(filePath, fontSize);
  if(!font_){
    std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(gApp.win); // TODO this isn't needed if below are removed
    TTF_Quit(); // TODO quit elsewhere, it doesnt belong here
    SDL_Quit(); // TODO quit elsewhere
    return true;
  }
  return false;
}

// Overload #1 with default color
void Font::Draw(const AppContext &gApp,
                int x, int y,
                const std::string &message) const
{
  SDL_Color color = {0,0,0,255};
  SDL_Rect dstRect = {x,y,0,0};
  Draw(gApp, x, y, message, dstRect, color);
}

// Overload #2 with custom color
void Font::Draw(const AppContext &gApp,
                int x, int y,
                const std::string &message,
                SDL_Color color) const
{
  SDL_Rect dstRect = {x,y,0,0};
  Draw(gApp, x, y, message, dstRect, color);
}

// Overload #3 with bounding box and alignment
void Font::Draw(const AppContext &gApp,
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

  if (SDL_BlitSurface(textSurface, nullptr, gApp.screen, &dstRect) < -1){
      SDL_Log("SDL_BlitSurface failed: %s", SDL_GetError());
  }

  SDL_FreeSurface(textSurface);
}
