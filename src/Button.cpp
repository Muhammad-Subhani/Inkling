#include "../include/Button.h"
#include <raylib.h>
void Button::Draw() const {
  if (onHover && !isPressed)
    DrawRectangleRounded(bound, roundness, segment, HOverColor);
  else if (isPressed || (onHover && isPressed))
    DrawRectangleRounded(bound, roundness, segment, SelectedCOlor);

  else
    DrawRectangleRounded(bound, roundness, segment, color);

  if (icon.id != 0) {
    float margin = 4.0f;
    float size = (bound.width < bound.height ? bound.width : bound.height) -
                 2.0f * margin;
    Rectangle source = {0.0f, 0.0f, (float)icon.width, (float)icon.height};
    Rectangle dest = {bound.x + (bound.width - size) / 2.0f,
                      bound.y + (bound.height - size) / 2.0f, size, size};
    DrawTexturePro(icon, source, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
  }
}
void Button::SetIcon(Texture2D iconTexture) { icon = iconTexture; }
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
void Button::ChangeIsPressedStatus(int val) { isPressed = val; }
