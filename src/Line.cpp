#include "../include/Line.h"
#include <algorithm>
#include <cmath>

void Line::Draw() const { DrawLineEx(a_, b_, thickness, color); }

bool Line::Contains(Vector2 p) const {
  return CheckCollisionPointLine(p, a_, b_, (int)(thickness / 2.0f + 4.0f));
}

bool Line::ClickedInBody(Vector2 p) const { return Contains(p); }

Rectangle Line::GetBounds() const {
  return {std::min(a_.x, b_.x), std::min(a_.y, b_.y),
          std::fabs(b_.x - a_.x), std::fabs(b_.y - a_.y)};
}

// Same contract as the other shapes: p is where the bounds' top-left should go.
void Line::MoveElement(Vector2 topLeft) {
  Rectangle r = GetBounds();
  Vector2 d = {topLeft.x - r.x, topLeft.y - r.y};
  a_.x += d.x; a_.y += d.y;
  b_.x += d.x; b_.y += d.y;
}