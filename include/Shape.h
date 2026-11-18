#pragma once
#include <raylib.h>

class Shape {
public:
  virtual ~Shape() = default;
  // its necessary to write the = 0
  // telling the compiler to place no body in the slot of the SHape Draw
  // so suppose you created new object and inherited Shape but forgot to write
  // the Draw with out = 0 in the Shape you cant figure ouit why nothing is
  // happening = 0 gives the error telling you that no Draw is implemented
  virtual void Draw() const = 0;
  int GetId() const { return _id; };
  virtual bool Contains(Vector2 p) const = 0;
  virtual Rectangle GetBounds() const = 0;

protected:
  Shape();
  Color color = WHITE;
  float thickness = 2;

private:
  int _id;
  static int nextId;
};
