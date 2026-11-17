#pragma once
#include "./BoundedShape.h"
#include "./Button.h"
#include <memory>
#include <raylib.h>
#include <vector>

namespace ToolbarConfig {
  // Spacing & Layout (tweak these variables to adjust sizing)
  constexpr float BUTTON_SIZE = 36.0f;       // Button size: 36px x 36px
  constexpr float BUTTON_RADIUS = 8.0f;      // Button border-radius: 8px
  constexpr float BUTTON_GAP = 4.0f;         // Gap between buttons: 4px
  constexpr float TOOLBAR_PADDING = 6.0f;    // Toolbar container padding: 6px
  constexpr float TOOLBAR_RADIUS = 12.0f;    // Toolbar container border-radius: 12px
  constexpr float TOOLBAR_LEFT = 16.0f;      // Left margin: 16px
  constexpr float STATUS_BAR_HEIGHT = 44.0f; // Status bar height for canvas vertical centering
  constexpr float ICON_SIZE = 20.0f;         // Icon bounding box: 20px x 20px
  constexpr float FONT_SIZE = 10.0f;         // Shortcut letter font size
  constexpr float TRANSITION_TIME = 0.120f;  // Smooth hover/active transition time (120ms)
}

class Bar : public BoundedShape {
  Color barcolor = {35, 35, 41, 255};
  Color borderColor = {55, 55, 65, 255};
  Color shadowColor = {0, 0, 0, 45};
  std::vector<std::unique_ptr<Button>> allButtons;
  float roundness = 0.5f;
  float segment = 12;
  int pressed = 2;
  void LayoutUpdate();
  void SetWhichButtonPressed();

public:
  Bar(Vector2 pos, float width = 48, float height = 0);
  void Draw() const override;
  void Add(std::unique_ptr<Button> S);
  void UpdateState(Vector2 mouse);
  void SelectAbutton(Vector2 mouse);
  void ChangePressed(int val) {
    pressed = val;
    SetWhichButtonPressed();
  };
  void SetPosition(Vector2 pos);
  Vector2 GetOrigin() const;
  Vector2 GetDimension() const;
  int GetPressed() const { return pressed; }
};
