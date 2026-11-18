#include "../include/Stroke.h"
#include <cstddef>
#include <raylib.h>
void Stroke::AddPoint(Vector2 p) { points.push_back(p); }
void Stroke::Draw() const {
  for (size_t i = 1; i < points.size(); i++)
    DrawLineEx(points[i - 1], points[i], thickness, color);
}
bool Stroke::Contains(Vector2 p) const {
  float tol = thickness / 2.0f + 4.0f;
  if (points.size() == 1)
    return CheckCollisionPointCircle(p, points[0], tol);
  for (size_t i = 1; i < points.size(); i++) {
    if (CheckCollisionPointLine(p, points[i - 1], points[i], int(tol)))
      return true;
  }
  return false;
}
