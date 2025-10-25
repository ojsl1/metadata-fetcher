#ifndef INPUT_H
#define INPUT_H
#pragma once // temp fix pp_including_mainfile_in_preamble for render.h including this file

#include "render.h"
#include "include/element.h"

class Mouse : public Element{
public:
  Mouse(int w, int h, const char* mouseImagePath = nullptr);
  SDL_Rect GetDrawRect() const;
  void Draw(AppContext gApp) const override;
  void GetXY() override;

  ~Mouse();

  SDL_Rect point;

private:
  SDL_Surface *rawMouse = nullptr;
  SDL_Surface *scaledMouse = nullptr;
  SDL_Rect dRectMouse;
};

#endif // INPUT_H
