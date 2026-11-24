#include "../include/ZoomIndicator.h"
#include "../include/Bar.h"
#include <cmath>
#include <raylib.h>
#include <string>

namespace {
constexpr float ZOOM_FONT_SIZE = 16.0f;
constexpr float ZOOM_TEXT_SPACING = 1.0f;
constexpr float ZOOM_PADDING_X = 14.0f;
constexpr float ZOOM_PADDING_Y = 8.0f;

std::string FormatZoomLabel(float zoom) {
  return std::to_string((int)std::round(zoom * 100.0f)) + "%";
}
} // namespace

void ZoomIndicator::UpdateLayout(float zoom) {
  std::string label = FormatZoomLabel(zoom);
  Vector2 textSize = MeasureTextEx(GetFontDefault(), label.c_str(),
                                   ZOOM_FONT_SIZE, ZOOM_TEXT_SPACING);
  float width = textSize.x + ZOOM_PADDING_X * 2.0f;
  float height = textSize.y + ZOOM_PADDING_Y * 2.0f;
  bounds = {(float)GetScreenWidth() - width - ToolbarConfig::TOOLBAR_LEFT,
            (float)GetScreenHeight() - height - ToolbarConfig::TOOLBAR_LEFT,
            width, height};
}

void ZoomIndicator::Draw(float zoom) const {
  DrawRectangleRounded(bounds, ToolbarConfig::PANEL_ROUNDNESS,
                       ToolbarConfig::PANEL_ROUND_SEGMENTS,
                       ToolbarConfig::PANEL_BG);
  std::string label = FormatZoomLabel(zoom);
  Vector2 textSize = MeasureTextEx(GetFontDefault(), label.c_str(),
                                   ZOOM_FONT_SIZE, ZOOM_TEXT_SPACING);
  Vector2 textPos = {bounds.x + (bounds.width - textSize.x) / 2.0f,
                     bounds.y + (bounds.height - textSize.y) / 2.0f};
  DrawTextEx(GetFontDefault(), label.c_str(), textPos, ZOOM_FONT_SIZE,
             ZOOM_TEXT_SPACING, ToolbarConfig::PANEL_TEXT);
}

bool ZoomIndicator::Contains(Vector2 point) const {
  return CheckCollisionPointRec(point, bounds);
}
