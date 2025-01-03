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
  Mouse(int w, int h, const char* mouseImagePath = NULL);
  SDL_Rect GetDrawRect() const; //helper to non-constify dRectMouse
  void Draw(SDL_Surface *gScreen) const override;
  void GetXY() override; // getter/updater for mouse coords

  ~Mouse();

  SDL_Rect point;

private:
  SDL_Surface *rawMouse = NULL;
  SDL_Surface *scaledMouse = NULL;
  SDL_Rect dRectMouse; // The size of the scaled cursor
};
   
  //TODO: cant privatize as button.cpp reads from point via Element::DetectIntersections()
  // implement a getter for getting the values:
  /*
  SDL_Rect point

  SDL_Rect getPoint() const {
    return point;
  }
  */

#endif // INPUT_H
