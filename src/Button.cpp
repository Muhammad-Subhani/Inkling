#include "../include/Button.h"
#include <raylib.h>
void Button::Draw() const {
  if (isPressed || onHover)
    DrawRectangleRounded(bound, roundness, segment, HOverColor);
  else
    DrawRectangleRounded(bound, roundness, segment, color);
}
float Button::getheight() const { return bound.height; }
float Button::getWitdh() const { return bound.width; }
Vector2 Button::GetOrigin() const { return Vector2{bound.x, bound.y}; }
void Button::setOrigin(Vector2 pos) {
  bound.x = pos.x;
  bound.y = pos.y;
}
Button::Button(Vector2 pos, float width, float height)
    : BoundedShape(pos, Vector2{pos.x + width, pos.y + height}) {}
void Button::isHovered(Vector2 mousePos) {
  onHover = CheckCollisionPointRec(mousePos, bound);
}
void Button::ChangeIsPressedStatus() { isPressed = !isPressed; }
