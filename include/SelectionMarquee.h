#include "BoundedShape.h"
#include <raylib.h>
class SelectionMarquee : public BoundedShape {
  const Color MARQUEE_BORDER = {166, 162, 235, 255};
  const Color MARQUEE_FILL = {166, 162, 235, 12}; // translucent, not solid
public:
  using BoundedShape::BoundedShape;
  void Draw() const override;
  bool Contains(Vector2 p) const override;
};
