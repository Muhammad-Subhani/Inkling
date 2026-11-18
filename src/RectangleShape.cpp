#include "../include/RectangleShape.h"
#include <raylib.h>
void RecatngleShape::Draw() const {
  DrawRectangleLinesEx(bound, thickness, color);
}
bool RecatngleShape::Contains(Vector2 p) const {
  return CheckCollisionPointRec(p, bound);
}
