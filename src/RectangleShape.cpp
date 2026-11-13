#include "../include/RectangleShape.h"
#include <raylib.h>
void RecatngleShape::Draw() const {
  DrawRectangleLinesEx(bound, thickness, color);
}
