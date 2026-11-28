#include "../include/Canvas.h"
#include <algorithm>
#include <raylib.h>
#include <utility>
#include <vector>
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
bool CheckMouseINBigSelection(Vector2 p, Rectangle temp) {
  return (p.x >= temp.x && p.x <= temp.x + temp.width && p.y >= temp.y &&
          p.y <= temp.y + temp.height);
}
void Canvas::ClearSelected() { selected.clear(); }
void Canvas::SetSelected(Vector2 p, bool ShiftLogic) {
  // cheking for multiple selections
  if (selected.size() > 1) {
    if (!CheckMouseINBigSelection(p, GetWholeARect()))
      ClearSelected();
    else
      return;
  }
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
Rectangle Canvas::GetWholeARect() const {
  float minX = selected[0]->GetBounds().x;
  float maxX = selected[0]->GetBounds().x + selected[0]->GetBounds().width;
  float minY = selected[0]->GetBounds().y;
  float maxY = selected[0]->GetBounds().y + selected[0]->GetBounds().height;
  for (auto &x : selected) {
    if (x->GetBounds().x + x->GetBounds().width > maxX)
      maxX = x->GetBounds().x + x->GetBounds().width;
    if (x->GetBounds().x < minX)
      minX = x->GetBounds().x;
    if (x->GetBounds().y + x->GetBounds().height > maxY)
      maxY = x->GetBounds().y + x->GetBounds().height;
    if (x->GetBounds().y < minY)
      minY = x->GetBounds().y;
  }
  return {minX, minY, maxX - minX, maxY - minY};
}
void Canvas::DrawBoundaries(float zoom) const {
  const Color SEL = {150, 145, 255, 255}; // Excalidraw-ish violet
  const float PAD = 6.0f / zoom;
  float thickness = 1.5f / zoom;
  for (const Shape *s : selected) {
    Rectangle b = s->GetBounds();
    Rectangle box = {b.x - PAD, b.y - PAD, b.width + 2 * PAD,
                     b.height + 2 * PAD};
    DrawRectangleLinesEx(box, thickness, SEL);
  }
  // if there are more than one elements copied we need a rectangle upon them ;
  if (selected.size() > 1) {
    Rectangle BigRect = GetWholeARect();
    BigRect = {BigRect.x - PAD, BigRect.y - PAD, BigRect.width + 2 * PAD,
               BigRect.height + 2 * PAD};
    DrawRectangleLinesEx(BigRect, thickness, SEL);
  }
}
void Canvas::CheckForEnclosingShapes(Rectangle temp) {
  for (auto &x : shapes) {
    Rectangle shapeBounds = x->GetBounds();
    if (temp.x <= shapeBounds.x &&
        temp.x + temp.width >= shapeBounds.width + shapeBounds.x &&
        temp.y <= shapeBounds.y &&
        temp.height + temp.y >= shapeBounds.height + shapeBounds.y)
      selected.push_back(x.get());
  }
}
void Canvas::MoveAllSelectedTogether(Vector2 p) {
  std::vector<Vector2> locs = LocationOfEachElementInbounds();
  int x = 0;
  for (auto &s : selected) {
    Vector2 newPos = {p.x + locs[x].x, p.y + locs[x].y};
    s->MoveElement(newPos);
    x++;
  }
}
std::vector<Vector2> Canvas::LocationOfEachElementInbounds() {
  std::vector<Vector2> ans;
  Rectangle temp = GetWholeARect();
  for (auto &s : selected) {
    ans.push_back(
        Vector2{s->GetBounds().x - temp.x, s->GetBounds().y - temp.y});
  }
  return ans;
}
Rectangle Canvas::GetUnionRectangleCAn(Rectangle a, Rectangle b) const {

  return {std::min(a.x, b.x), std::min(a.y, b.y),
          std::max(a.x + a.width, b.x + b.width) - std::min(a.x, b.x),
          std::max(a.y + a.height, b.y + b.height) - std::min(a.y, b.y)};
}
Rectangle Canvas::GetWorldBound() const {
  Rectangle temp;
  if (shapes.size() == 0)
    return {0, 0, 0, 0};
  temp = shapes[0]->GetBounds();
  for (auto &s : shapes) {
    temp = GetUnionRectangleCAn(temp, s->GetBounds());
  }
  return temp;
}
void Canvas::DrawMiniMap() const {
  for (auto &s : shapes) {
    DrawRectangleRec(s->GetBounds(), WHITE);
  }
}
