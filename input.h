#ifndef INPUT_H
#define INPUT_H
#pragma once

#include "main.h" // globals
#include "element.h"
#include "render.h"
#include <iostream>

class Mouse : public Element{
public:
  Mouse(int w, int h, const char* mouseImagePath = nullptr);
  SDL_Rect GetDrawRect() const;
  void Draw(SDL_Surface *gScreen) const override;
  void GetXY() override;

  ~Mouse();

  SDL_Rect point;

private:
  SDL_Surface *rawMouse = nullptr;
  SDL_Surface *scaledMouse = nullptr;
  SDL_Rect dRectMouse;
};

#endif // INPUT_H
