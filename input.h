#ifndef INPUT_H
#define INPUT_H
#pragma once

#include "main.h" // globals
#include "element.h"
#include "render.h"
#include <iostream>

class Mouse : public Element{
public:
  // Optional filepath for custom cursor
  Mouse(int w, int h, const char* mouseImagePath = nullptr);
  SDL_Rect GetDrawRect() const; //helper to non-constify dRectMouse
  void Draw(SDL_Surface *gScreen) const override;
  void GetXY() override; // getter/updater for mouse coords

  ~Mouse();

  SDL_Rect point;

private:
  SDL_Surface *rawMouse = nullptr;
  SDL_Surface *scaledMouse = nullptr;
  SDL_Rect dRectMouse; // The size of the scaled cursor
};

#endif // INPUT_H
