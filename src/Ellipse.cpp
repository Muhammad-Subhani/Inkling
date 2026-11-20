#include "../include/Ellipse.h"
void Ellipse::Draw() const {
  DrawEllipseLines((int)(bound.x + bound.width / 2),
                   (int)(bound.y + bound.height / 2), bound.width / 2,
                   bound.height / 2, color);
}
bool Ellipse::Contains(Vector2 p) const {
  // how to detect if the point is in the ellipse or not
  // (x-h)^2 /radiiX^2 + (y-k)^2 / radiiY^2 <= 1
  // just put the points in the x and y
  float radiiX = bound.width / 2;
  float radiiY = bound.height / 2;
  float dx = (p.x - (bound.x + radiiX)) / radiiX;
  float dy = (p.y - (bound.y + radiiY)) / radiiY;
  return dx * dx + dy * dy <= 1.0;
}
bool Ellipse::ClickedInBody(Vector2 p) const {
  float radiiX = bound.width / 2;
  float radiiY = bound.height / 2;
  float dx = (p.x - (bound.x + radiiX)) / radiiX;
  float dy = (p.y - (bound.y + radiiY)) / radiiY;
  return dx * dx + dy * dy < 1.0;
}
