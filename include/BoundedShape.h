#pragma once
#include "Shape.h"
#include <raylib.h>
class BoundedShape : public Shape {
public:
  BoundedShape(Vector2 a, Vector2 b);
  void SetCorners(Vector2 a, Vector2 b);
  Rectangle GetBounds() const override;
  bool ClickedInBody(Vector2 p) const override;
  void MoveElement(Vector2 p) override;

protected:
  Rectangle bound;
  bool Contains(Vector2 p) const override;
};
