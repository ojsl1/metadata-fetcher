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

  // Overload #1 default color
  void Draw(SDL_Surface *gScreen, int x, int y, const std::string &message);

  // Overload #2 custom color
  void Draw(SDL_Surface *gScreen, int x, int y, const std::string &message, SDL_Color color_);

  // Overload #3 With bounding box or alignment 
  void Draw(SDL_Surface *gScreen, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color_);

private:
    TTF_Font *font_;
    std::string message;
    SDL_Color color_;
    SDL_Rect dstRect;

    //TODO also store default color,style,etc.
};


#endif // FONT_H
