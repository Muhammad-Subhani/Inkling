#include "../include/Bar.h"
#include <cstddef>
#include <memory>
#include <raylib.h>
#include <utility>
const int pad = 10;
const int gap = 13;
Bar::Bar(Vector2 pos, float width, float height)
    : BoundedShape(pos, Vector2{pos.x + width, pos.y + height}) {};
void Bar::Draw() const {
  DrawRectangleRounded(bound, roundness, segment, barcolor);
  for (const auto &item : allButtons) {
    item->Draw();
  }
}
void Bar::LayoutUpdate() {
  if (allButtons.size() <= 0)
    return;
  bound.height = allButtons.size() * allButtons[0]->getheight() +
                 (allButtons.size() - 1) * gap + 2 * pad;
  bound.width = 2 * pad + allButtons[0]->getWitdh();

  for (size_t i = 0; i < allButtons.size(); i++) {
    allButtons[i]->setOrigin(
        Vector2{bound.x + pad,
                bound.y + pad + (i * (allButtons[0]->getheight() + gap))});
  }
}
void Bar::Add(std::unique_ptr<Button> S) {
  allButtons.push_back(std::move(S));
  LayoutUpdate();
}
void Bar::UpdateState(Vector2 mouse) {
  for (auto &p : allButtons) {
    p->isHovered(mouse);
  }
}
void Bar::SetWhichButtonPressed() {
  for (size_t i = 0; i < allButtons.size(); i++) {
    if (i != (size_t)pressed)
      allButtons[i]->ChangeIsPressedStatus(0);
    else
      allButtons[i]->ChangeIsPressedStatus(1);
  }
}
void Bar::SelectAbutton(Vector2 mousePos) {

  if (mousePos.x >= bound.x && mousePos.x <= bound.x + bound.width &&
      mousePos.y >= bound.y && mousePos.y <= bound.y + bound.height) {
    for (size_t i = 0; i < allButtons.size(); i++) {
      if (mousePos.x >= allButtons[i]->GetOrigin().x &&
          mousePos.x <=
              allButtons[i]->GetOrigin().x + allButtons[i]->getWitdh() &&
          mousePos.y >= allButtons[i]->GetOrigin().y &&
          mousePos.y <=
              allButtons[i]->GetOrigin().y + allButtons[i]->getheight()) {
        ChangePressed(i);
        break;
      }
    }
  }
}
Vector2 Bar::GetOrigin() const { return Vector2{bound.x, bound.y}; }
Vector2 Bar::GetDimension() const { return Vector2{bound.width, bound.height}; }
