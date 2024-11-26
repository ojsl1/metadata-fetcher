#ifndef MATH_H
#define MATH_H

// VECTOR-RELATED FUNCTIONS

/*
 * vect2 provides a lightweight 2D vector class for operations such as
 * addition, scaling, normalization, and dot products. Useful for
 * managing positions, velocities, and directions in 2D space.
*/

/* Performance Tips and Notes:
 * Avoid Repeated Normalization: Normalization can be computationally
 * intensive. Cache the normalized vector if it will be reused frequently.
 * Use Temporary Variables for Large Calculations: When chaining operations,
 * intermediate temporary variables can help keep the code clear and performant.
*/

/* Memory Layout:
 * Vect2 is small and can be passed by value, but for larger structures,
 * prefer passing by reference.
*/

/* Future Improvements (if relevant):
 * Consider adding additional vector operations, such as
 * cross-product for 3D, or conversion functions if interfacing with
 * libraries that have their own vector types.
*/

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

#endif // MATH_H
