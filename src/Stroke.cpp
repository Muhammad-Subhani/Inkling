#include "../include/Stroke.h"
#include <cstddef>
#include <iostream>
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

Rectangle Stroke::GetBounds() const {
  if (points.empty())
    return {0, 0, 0, 0};
  float minX = points[0].x, maxX = points[0].x;
  float minY = points[0].y, maxY = points[0].y;
  for (const Vector2 &p : points) {
    if (p.x < minX)
      minX = p.x;
    if (p.x > maxX)
      maxX = p.x;
    if (p.y < minY)
      minY = p.y;
    if (p.y > maxY)
      maxY = p.y;
  }
  return {minX, minY, maxX - minX, maxY - minY};
}
bool Stroke::ClickedInBody(Vector2 p) const {
  // Rectangle temp = GetBounds();
  // return (p.x > temp.x && p.x < temp.x + temp.width && p.y > temp.y &&
  //         p.y < temp.y + temp.height);
  return Contains(p);
}
void Stroke::MoveElement(Vector2 p) {
  Rectangle temp = GetBounds();
  // one thing to make sure that you are not using auto here
  // auto makes the copy so you are actusally not changing the actaull point use
  // referenece
  for (Vector2 &point : points) {
    Vector2 diff = {point.x - temp.x, point.y - temp.y};
    point.x = p.x + diff.x;
    point.y = p.y + diff.y;
  }
}
