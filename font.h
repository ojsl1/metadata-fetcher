#ifndef FONT_H
#define FONT_H
#include "main.h" // globals
#include <string>
#include <iostream>

class Font{
public:
  Font();
  ~Font();

  bool Load(const char *filePath, int fontSize);
  void Free();
  void Shutdown();

  void Draw(SDL_Surface *gScreen, int x, int y, const std::string &message);
  void Draw(SDL_Surface *gScreen, int x, int y, const std::string &message, SDL_Color color_);
  void Draw(SDL_Surface *gScreen, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color_);

private:
    TTF_Font *font_;
    std::string message;
    SDL_Color color_;
    SDL_Rect dstRect;

    //TODO also store default color,style,etc.
};


#endif // FONT_H
