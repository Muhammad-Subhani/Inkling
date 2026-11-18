#include "../include/BoundedShape.h"
#include <algorithm>
#include <cmath>
#include <raylib.h>
BoundedShape::BoundedShape(Vector2 a, Vector2 b) {
  BoundedShape::SetCorners(a, b);
}
void BoundedShape::SetCorners(Vector2 a, Vector2 b) {
  bound = {std::min(a.x, b.x), std::min(a.y, b.y), std::fabs(b.x - a.x),
           std::fabs(b.y - a.y)};
}
bool BoundedShape::Contains(Vector2 p) const {
  return CheckCollisionPointRec(p, bound);
}
