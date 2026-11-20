#pragma once
#include "BoundedShape.h"
#include <raylib.h>
class Ellipse : public BoundedShape {
public:
  using BoundedShape::BoundedShape;
  void Draw() const override;
  bool Contains(Vector2 p) const override;
  bool ClickedInBody(Vector2 p) const override;
};
