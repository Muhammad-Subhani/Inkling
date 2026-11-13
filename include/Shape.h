#pragma once
#include <raylib.h>

class Shape {
public:
  virtual ~Shape() = default;
  virtual void Draw() const = 0;
  int GetId() const { return _id; };

protected:
  Shape();
  Color color = WHITE;
  float thickness = 2;

private:
  int _id;
  static int nextId;
};
