#include "../include/Ellipse.h"
void Ellipse::Draw() const {
  DrawEllipseLines((int)(bound.x + bound.width / 2),
                   (int)(bound.y + bound.height / 2), bound.width / 2,
                   bound.height / 2, color);
}
