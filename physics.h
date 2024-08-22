#ifndef PHYSICS_H
#define PHYSICS_H

//OOP: Base class
class AppObject {
public:
  AppObject(
};

//OOP: Subclasses
class Button : public AppObject {
public:
};
class Mouse : public AppObject {
public:
};

class Vect2 {
private:
  float x;
  float y;

public:
  Vect2() : x(0), y(0) {} // Default constructor
  Vect2(float xVal, float yVal) : x(xVal), y(yVal) {} // Parameterized constructor

  // Getter methods
  float getX() const { return x; }
  float getY() const { return y; }

  // Setter methods
  void setX(float xVal) { x = xVal; }
  void setY(float yVal) { x = yVal; }

  // Other member functions
  float magnitude() const { return sqrt(x * x + y * y); }
  void normalize(){
    float mag = magnitude();
    if (mag > 0) {
      x /= mag;
      y /= mag;
    }
  }
};

#endif // PHYSICS_H
