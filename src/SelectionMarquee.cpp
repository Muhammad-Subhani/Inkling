#include "../include/SelectionMarquee.h"
#include <raylib.h>
void SelectionMarquee::Draw() const {
  DrawRectangleRec(bound, MARQUEE_FILL);
  DrawRectangleLinesEx(bound, 2, MARQUEE_BORDER);
}
bool SelectionMarquee::Contains(Vector2 p) const {
  return CheckCollisionPointRec(p, bound);
}
