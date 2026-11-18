#include "../include/Diamond.h"
#include <cmath>
#include <raylib.h>
void Diamond::Draw() const {
  float cx = bound.x + bound.width / 2;
  float cy = bound.y + bound.height / 2;
  Vector2 top = {cx, bound.y};
  Vector2 right = {bound.x + bound.width, cy};
  Vector2 left = {bound.x, cy};
  Vector2 down = {cx, bound.height + bound.y};
  DrawLineEx(top, right, thickness, color);
  DrawLineEx(right, down, thickness, color);
  DrawLineEx(down, left, thickness, color);
  DrawLineEx(left, top, thickness, color);
}
bool Diamond::Contains(Vector2 p) const {
  // here using the rhombus formulae
  // |x-h|/rx + |y-k|/ry <=1
  float radiiX = bound.width / 2.0;
  float radiiY = bound.height / 2.0;
  float dx = std::fabsf(p.x - (bound.x + radiiX)) / radiiX;
  float dy = std::fabsf(p.y - (bound.y + radiiY)) / radiiY;
  return dx + dy <= 1.0;
}
