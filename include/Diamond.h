#pragma once
#include "BoundedShape.h"
#include <raylib.h>
class Diamond : public BoundedShape {
  using BoundedShape::BoundedShape;
  void Draw() const override;
};
