#include "../include/Button.h"
#include "../include/Bar.h"
#include <algorithm>
#include <raylib.h>

static Color LerpColor(Color a, Color b, float t) {
  if (t <= 0.0f) return a;
  if (t >= 1.0f) return b;
  return Color{
    (unsigned char)(a.r + (b.r - a.r) * t),
    (unsigned char)(a.g + (b.g - a.g) * t),
    (unsigned char)(a.b + (b.b - a.b) * t),
    (unsigned char)(a.a + (b.a - a.a) * t)
  };
}

Button::Button(Vector2 pos, float width, float height)
    : BoundedShape(pos, Vector2{pos.x + width, pos.y + height}) {
  roundness = ToolbarConfig::TOOL_BTN_RADIUS / (width / 2.0f);
}

void Button::Draw() const {
  // Determine animated background color with 120ms transition
  Color currentBg = color;
  if (activeProgress > 0.0f) {
    Color activeBg = hoverProgress > 0.0f
      ? LerpColor(selectedColor, Color{74, 72, 118, 255}, hoverProgress)
      : selectedColor;
    currentBg = LerpColor(color, activeBg, activeProgress);
  } else if (hoverProgress > 0.0f) {
    currentBg = LerpColor(color, hoverColor, hoverProgress);
  }

  // Draw button background
  if (currentBg.a > 0) {
    DrawRectangleRounded(bound, roundness, segment, currentBg);
  }

  // Draw 22px icon centered in button, translated up by 2px (translateY(-2px)) to guarantee no collision with label
  if (icon.id != 0) {
    Color currentIconColor = iconIdleColor;
    if (activeProgress > 0.0f) {
      currentIconColor = LerpColor(iconIdleColor, iconActiveColor, activeProgress);
    } else if (hoverProgress > 0.0f) {
      currentIconColor = LerpColor(iconIdleColor, iconHoverColor, hoverProgress);
    }

    float iconSize = ToolbarConfig::TOOL_ICON_SIZE;
    Rectangle source = {0.0f, 0.0f, (float)icon.width, (float)icon.height};
    Rectangle dest = {
      bound.x + (bound.width - iconSize) / 2.0f,
      bound.y + (bound.height - iconSize) / 2.0f + ToolbarConfig::TOOL_ICON_OFFSET_Y,
      iconSize,
      iconSize
    };
    DrawTexturePro(icon, source, dest, Vector2{0.0f, 0.0f}, 0.0f, currentIconColor);
  }

  // Draw shortcut label positioned absolute at bottom: 3px; right: 5px
  if (!shortcut.empty()) {
    Color currentTextColor = textIdleColor;
    if (activeProgress > 0.0f) {
      currentTextColor = LerpColor(textIdleColor, textActiveColor, activeProgress);
    } else if (hoverProgress > 0.0f) {
      currentTextColor = LerpColor(textIdleColor, textHoverColor, hoverProgress);
    }

    int fontSize = (int)ToolbarConfig::TOOL_LABEL_SIZE;
    int textW = MeasureText(shortcut.c_str(), fontSize);
    int tx = (int)(bound.x + bound.width - textW - ToolbarConfig::TOOL_LABEL_RIGHT);
    int ty = (int)(bound.y + bound.height - fontSize - ToolbarConfig::TOOL_LABEL_BOTTOM);
    DrawText(shortcut.c_str(), tx, ty, fontSize, currentTextColor);
  }
}

void Button::isHovered(Vector2 mousePos) {
  onHover = CheckCollisionPointRec(mousePos, bound);

  float dt = GetFrameTime();
  if (dt <= 0.0f || dt > 0.1f) dt = 1.0f / 60.0f;
  float speed = dt / ToolbarConfig::TRANSITION_TIME; // 120ms transition duration

  if (onHover)
    hoverProgress = std::min(1.0f, hoverProgress + speed);
  else
    hoverProgress = std::max(0.0f, hoverProgress - speed);

  if (isPressed)
    activeProgress = std::min(1.0f, activeProgress + speed);
  else
    activeProgress = std::max(0.0f, activeProgress - speed);
}

void Button::ChangeIsPressedStatus(int val) { isPressed = val; }
void Button::SetIcon(Texture2D iconTexture) { icon = iconTexture; }
void Button::SetShortcut(const std::string &s) { shortcut = s; }
void Button::SetTooltip(const std::string &t) { tooltip = t; }
const std::string &Button::GetShortcut() const { return shortcut; }
const std::string &Button::GetTooltip() const { return tooltip; }
bool Button::IsHovered() const { return onHover; }
float Button::getheight() const { return bound.height; }
float Button::getWitdh() const { return bound.width; }
Vector2 Button::GetOrigin() const { return Vector2{bound.x, bound.y}; }
void Button::setOrigin(Vector2 pos) {
  bound.x = pos.x;
  bound.y = pos.y;
}
