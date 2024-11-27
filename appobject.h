#ifndef PHYSICS_H
#define PHYSICS_H


//OOP: Base class
class AppObject {
protected:
  float posX, posY;
  bool isActive;

public:
  //TODO:
  AppObject() : posX(0), posY(0), isActive(true) {}

  //TODO: Position handling
  void setPosition(float newX, float newY) { posX = newX; posY = newY; }
  std::pair<float, float> getPosition() const { return {posX, posY}; }

  //TODO: Active state
  void setActive(bool state) { isActive = state; }
  bool getActive() const { return isActive; }

  //TODO REFACTOR: Ensure subclasses call these without errors
  virtual void Draw(SDL_Surface *gScreen) const = 0;
  virtual void Update() = 0;

  //TODO: Event handling (optional virtual functions)
  virtual void onClick() {}
  virtual void onHover() {}

  //TODO: Ensure a virtual destructor for polymorphism
  virtual ~AppObject() = default;
};

#endif // PHYSICS_H
