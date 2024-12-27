#ifndef INPUT_H
#define INPUT_H
#pragma once

#include "main.h" // globals
#include "appobject.h"
#include "render.h"
#include <iostream>

class Mouse : public AppObject{
public:
  // Optional filepath for custom cursor
  Mouse(int w, int h, const char* mouseImagePath = NULL);
  SDL_Rect GetDrawRect() const;
  void Draw(SDL_Surface *gScreen) const override;
  void Update() override; // get mouse state
  void UpdateMouseState(SDL_Event &e); // Update mouse 
  ~Mouse();

  int x, y; // Mouse position
  bool clicked;
  SDL_Rect point;

private:
  SDL_Surface* rawMouse = NULL;
  SDL_Surface* scaledMouse = NULL;
  SDL_Rect dRectMouse; // The size of the scaled cursor
};
   
  //TODO: cant privatize as button.cpp reads from point via Button::DetectIntersections()
  // implement a getter for getting the values:
  /*
  SDL_Rect point

  SDL_Rect getPoint() const {
    return point;
  }
  */

#endif // INPUT_H
