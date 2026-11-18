#pragma once
#include "Shape.h"
#include <raylib.h>
#include <vector>
class Stroke : public Shape {
public:
  void AddPoint(Vector2 p);
  void Draw() const override;
  bool Contains(Vector2 p) const override;

  Rectangle GetBounds() const override;

private:
  std::vector<Vector2> points;
  Color color = WHITE;
  float thickness = 2;
};
