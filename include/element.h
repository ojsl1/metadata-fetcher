#ifndef ELEMENT_H
#define ELEMENT_H

#include "main.h" // sdl

class Element {
protected:
  float posX, posY;
  bool isActive;

public:
  Element() : posX(0), posY(0), isActive(true) {}

  //WIP: Position handling
  void setPosition(float newX, float newY) { posX = newX; posY = newY; }
  std::pair<float, float> getPosition() const { return {posX, posY}; }

  //WIP: Active state
  void setActive(bool state) { isActive = state; }
  bool getActive() const { return isActive; }

  //WIP REFACTOR: Ensure subclasses call these without errors
  virtual void Draw(AppContext gApp) const = 0;
  //virtual void Update() = 0;
  virtual void GetXY() = 0;

  //WIP: Event handling (optional virtual functions)
  virtual void onClick() {}
  virtual void onHover() {}

  //WIP: Ensure a virtual destructor for polymorphism
  virtual ~Element() = default;
};

#endif // ELEMENT_H
