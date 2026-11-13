#include "../include/Stroke.h"
void Stroke::AddPoint(Vector2 p) { points.push_back(p); }
void Stroke::Draw() const {
  for (size_t i = 1; i < points.size(); i++)
    DrawLineEx(points[i - 1], points[i], thickness, color);
}
