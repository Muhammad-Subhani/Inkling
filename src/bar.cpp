#include "../include/Bar.h"
#include <cstddef>
#include <memory>
#include <raylib.h>
#include <utility>

Bar::Bar(Vector2 pos, float width, float height)
    : BoundedShape(pos, Vector2{pos.x + width, pos.y + height}) {
  roundness = ToolbarConfig::TOOLBAR_RADIUS /
              ((2 * ToolbarConfig::TOOLBAR_PADDING + ToolbarConfig::BUTTON_SIZE) / 2.0f);
}

void Bar::Draw() const {
  // Soft elevation shadow
  Rectangle shadowRec = {
    bound.x - 1.0f, bound.y + 2.0f, bound.width + 2.0f, bound.height + 4.0f
  };
  DrawRectangleRounded(shadowRec, roundness, (int)segment, shadowColor);

  // Dark toolbar background
  DrawRectangleRounded(bound, roundness, (int)segment, barcolor);

  // Subtle 1px border
  DrawRectangleRoundedLinesEx(bound, roundness, (int)segment, 1.0f, borderColor);

  // Draw buttons
  for (const auto &item : allButtons) {
    item->Draw();
  }

  // Draw tooltip for hovered button on top of all elements
  for (const auto &item : allButtons) {
    if (item->IsHovered() && !item->GetTooltip().empty()) {
      float fontSize = 11.0f;
      int textWidth = MeasureText(item->GetTooltip().c_str(), (int)fontSize);
      float padX = 8.0f;
      float padY = 4.0f;
      float ttWidth = textWidth + 2 * padX;
      float ttHeight = fontSize + 2 * padY;
      float ttX = bound.x + bound.width + 10.0f;
      float ttY = item->GetOrigin().y + (item->getheight() - ttHeight) / 2.0f;
      Rectangle ttRec = { ttX, ttY, ttWidth, ttHeight };
      DrawRectangleRounded(ttRec, 0.35f, 6, Color{24, 24, 28, 245});
      DrawRectangleRoundedLinesEx(ttRec, 0.35f, 6, 1.0f, Color{55, 55, 65, 255});
      DrawText(item->GetTooltip().c_str(), (int)(ttX + padX), (int)(ttY + padY),
               (int)fontSize, Color{240, 240, 245, 255});
      break;
    }
  }
}

void Bar::LayoutUpdate() {
  if (allButtons.empty())
    return;
  float pad = ToolbarConfig::TOOLBAR_PADDING;
  float gap = ToolbarConfig::BUTTON_GAP;
  float btnH = allButtons[0]->getheight();
  float btnW = allButtons[0]->getWitdh();

  bound.height = allButtons.size() * btnH +
                 (allButtons.size() - 1) * gap + 2 * pad;
  bound.width = 2 * pad + btnW;
  roundness = ToolbarConfig::TOOLBAR_RADIUS / (bound.width / 2.0f);

  for (size_t i = 0; i < allButtons.size(); i++) {
    allButtons[i]->setOrigin(
        Vector2{bound.x + pad,
                bound.y + pad + (i * (btnH + gap))});
  }
}

void Bar::SetPosition(Vector2 pos) {
  bound.x = pos.x;
  bound.y = pos.y;
  LayoutUpdate();
}

void Bar::Add(std::unique_ptr<Button> S) {
  allButtons.push_back(std::move(S));
  LayoutUpdate();
}

void Bar::UpdateState(Vector2 mouse) {
  bool anyHovered = false;
  for (auto &p : allButtons) {
    p->isHovered(mouse);
    if (p->IsHovered()) {
      anyHovered = true;
    }
  }
  if (anyHovered) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  } else {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
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
