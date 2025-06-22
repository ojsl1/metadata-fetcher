#include "font.h"

Font::Font()
  : font_(nullptr)
{
  //this->color_ = color_; // set the text color (?)
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
    std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    SDL_DestroyWindow(gWindow); // TODO this isn't needed if below are removed
    TTF_Quit(); // TODO quit elsewhere, it doesnt belong here
    SDL_Quit(); // TODO quit elsewhere
    return 1;
  }
  return 0;
}

// Overload #1 with default color
void Font::Draw(SDL_Surface *gScreen, int x, int y, const std::string &message){
  color_ = {000,000,000,000};
  dstRect = {x,y,0,0};
  Draw(gScreen, x, y, message, dstRect, color_);
}

// Overload #2 with custom color
void Font::Draw(SDL_Surface *gScreen, int x, int y, const std::string &message, SDL_Color color_){
  dstRect = {x,y,0,0};
  Draw(gScreen, x, y, message, dstRect, color_);
}

// Overload #3 with bounding box and alignment 
void Font::Draw(SDL_Surface *gScreen, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color_){
  if(!font_){
    std::cerr << "No font loaded, cannot render text!.\n";
    return;
  }

  //Alternatively: for antialiased text use TTF_RenderText_Blended:
  //SDL_Surface *textSurface = TTF_RenderText_Blended(font_, message.c_str(), color_);

  SDL_Surface *textSurface = TTF_RenderText_Solid(font_, message.c_str(), color_);
  if(!textSurface){
    std::cerr << "TTF_RenderText_Solid failed: " << TTF_GetError() << std::endl;
    return;
  }

  dstRect.x = x;
  dstRect.y = y;

  if (SDL_BlitSurface(textSurface, nullptr, gScreen, &dstRect) < 0){
      SDL_Log("SDL_BlitSurface failed: %s", SDL_GetError());
  }

  SDL_FreeSurface(textSurface);
}
