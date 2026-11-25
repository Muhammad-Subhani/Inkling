#pragma once
#include "./BoundedShape.h"
#include "./Button.h"
#include <memory>
#include <raylib.h>
#include <vector>

namespace ToolbarConfig {
// =========================================================================
// Toolbar Sizing & Layout Configuration (CSS-like Design Tokens)
// Tweak these variables to adjust sizing, spacing, positioning, and opacity:
// =========================================================================

// --tool-btn-size: Button width and height (44px)
constexpr float TOOL_BTN_SIZE = 44.0f;
constexpr float BUTTON_SIZE = TOOL_BTN_SIZE; // alias

// --tool-btn-radius: Button corner border-radius (10px)
constexpr float TOOL_BTN_RADIUS = 10.0f;
constexpr float BUTTON_RADIUS = TOOL_BTN_RADIUS; // alias

// --tool-gap: Gap between buttons (6px, ~50px per slot: 44px + 6px = 50px)
constexpr float TOOL_GAP = 9.0f;
constexpr float BUTTON_GAP = TOOL_GAP; // alias

// --toolbar-padding: Toolbar container padding (8px)
constexpr float TOOLBAR_PADDING = 8.0f;

// --toolbar-radius: Toolbar container border-radius (14px)
constexpr float TOOLBAR_RADIUS = 14.0f;

// --toolbar-left: Toolbar left offset from window edge (16px)
constexpr float TOOLBAR_LEFT = 16.0f;

// --tool-icon-size: Icon size inside button (22px)
constexpr float TOOL_ICON_SIZE = 22.0f;
constexpr float ICON_SIZE = TOOL_ICON_SIZE; // alias

// --tool-icon-offset-y: translateY(-2px) to guarantee no collision with
// shortcut label
constexpr float TOOL_ICON_OFFSET_Y = -2.0f;

// --tool-label-size: Shortcut label font size (11px)
constexpr float TOOL_LABEL_SIZE = 11.0f;
constexpr float FONT_SIZE = TOOL_LABEL_SIZE; // alias

// --tool-label-bottom: Label absolute bottom offset (3px)
constexpr float TOOL_LABEL_BOTTOM = 3.0f;

// --tool-label-right: Label absolute right offset (5px)
constexpr float TOOL_LABEL_RIGHT = 5.0f;

// --tool-label-opacity-inactive: Shortcut label opacity on inactive buttons
// (0.65)
constexpr float TOOL_LABEL_OPACITY_INACTIVE = 0.65f;

// --tool-label-opacity-active: Shortcut label opacity on active button (0.95)
constexpr float TOOL_LABEL_OPACITY_ACTIVE = 0.95f;

// --top-bar-height: Height of top status/selection bar for vertical centering
// (0.0f if none)
constexpr float TOP_BAR_HEIGHT = 0.0f;
constexpr float STATUS_BAR_HEIGHT = TOP_BAR_HEIGHT; // alias

// --transition-time: Smooth hover/active transition duration (120ms)
constexpr float TRANSITION_TIME = 0.120f;

// Shared panel styling for screen-space UI panels
constexpr Color PANEL_BG = {35, 35, 41, 255};
constexpr Color PANEL_TEXT = {240, 240, 245, 255};
constexpr float PANEL_ROUNDNESS = 0.2f;
constexpr int PANEL_ROUND_SEGMENTS = 8;
} // namespace ToolbarConfig

class Bar : public BoundedShape {
  Color barcolor = {35, 35, 41, 255};
  Color borderColor = {55, 55, 65, 255};
  Color shadowColor = {0, 0, 0, 45};
  std::vector<std::unique_ptr<Button>> allButtons;
  float roundness = 0.4667f;
  float segment = 12;
  int pressed = 2;
  void LayoutUpdate();
  void SetWhichButtonPressed();

public:
  Bar(Vector2 pos,
      float width = 2 * ToolbarConfig::TOOLBAR_PADDING +
                    ToolbarConfig::TOOL_BTN_SIZE,
      float height = 0);
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
