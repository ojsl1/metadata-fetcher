#ifndef FONT_H
#define FONT_H

#include "main.h" // sdl
#include <string>

class Font{
public:
  Font();
  ~Font();

  bool Load(const char *filePath, int fontSize);
  void Free();
  void Shutdown();

  void Draw(const AppContext &gApp, int x, int y, const std::string &message) const;
  void Draw(const AppContext &gApp, int x, int y, const std::string &message, SDL_Color color) const;
  void Draw(const AppContext &gApp, int x, int y, const std::string &message, SDL_Rect dstRect, SDL_Color color) const;

private:
  TTF_Font *font_;

  //TODO store default properties
  //SDL_Color defaultColor_;
  //int defaultSize;

};

#endif // FONT_H
