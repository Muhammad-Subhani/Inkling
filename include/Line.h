#pragma once
#include "Shape.h"
#include <raylib.h>

class Line : public Shape {
public:
  Line(Vector2 a, Vector2 b) : a_(a), b_(b) {}
  void Draw() const override;
  bool Contains(Vector2 p) const override;
  bool ClickedInBody(Vector2 p) const override;
  Rectangle GetBounds() const override;
  void MoveElement(Vector2 topLeft) override;

private:
  Vector2 a_, b_;
};