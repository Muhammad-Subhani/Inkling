#pragma once
#include "./BoundedShape.h"
class RecatngleShape : public BoundedShape {
public:
  using BoundedShape::BoundedShape;
  void Draw() const override;
};
