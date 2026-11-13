#pragma once
#include <raylib.h>
#include <vector>
class Stroke {
public:
  void AddPoint(Vector2 p);
  void Draw() const;

private:
  std::vector<Vector2> points;
  Color color = WHITE;
  float thickness = 2;
};
