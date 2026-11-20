#include "../include/Canvas.h"
#include <algorithm>
#include <raylib.h>
void Canvas::Add(std::unique_ptr<Shape> s) { shapes.push_back(std::move(s)); }
void Canvas::Draw() const {
  for (const auto &s : shapes)
    s->Draw();
}
Shape *Canvas::PickAt(Vector2 p) const {
  for (auto &x : shapes) {
    if (x->Contains(p))
      return x.get();
  }
  return nullptr;
}
Shape *Canvas::BodyClickedCheck(Vector2 p) const {
  for (auto &x : shapes) {
    if (x->ClickedInBody(p))
      return x.get();
  }
  return nullptr;
}
bool Canvas::IsSelected(Shape *s) const {
  // the end is not the last element its the one past the last element
  // so end () is returned when the find didnt get your shape
  // and this check is what returns either true or false
  return std::find(selected.begin(), selected.end(), s) != selected.end();
}
void Canvas::ClearSelected() { selected.clear(); }
void Canvas::SetSelected(Vector2 p, bool ShiftLogic) {
  Shape *hitted = PickAt(p);
  if (!hitted) {
    if (!ShiftLogic)
      this->ClearSelected();
    return;
  } else if (ShiftLogic) {
    if (IsSelected(hitted))
      selected.erase(std::remove(selected.begin(), selected.end(), hitted),
                     selected.end());

    else
      selected.push_back(hitted);
  } else {
    ClearSelected();
    selected.push_back(hitted);
  }
}
void Canvas::DrawBoundaries() const {
  const Color SEL = {150, 145, 255, 255}; // Excalidraw-ish violet
  const float PAD = 6.0f;
  for (const Shape *s : selected) {
    Rectangle b = s->GetBounds();
    Rectangle box = {b.x - PAD, b.y - PAD, b.width + 2 * PAD,
                     b.height + 2 * PAD};
    DrawRectangleLinesEx(box, 1.5f, SEL);
  }
}
