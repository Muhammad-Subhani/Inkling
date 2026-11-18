#pragma once
#include "./BoundedShape.h"
#include <raylib.h>
#include <string>

class Button : public BoundedShape {
  // Excalidraw-inspired color palette
  Color color = {35, 35, 41, 0}; // Idle background (matches bar container)
  Color hoverColor = {
      255, 255, 255,
      20}; // Hover background (~8% white: rgba(255, 255, 255, 0.08))
  Color selectedColor = {64, 62, 106,
                         255}; // Active purple-ish background (#403e6a)

  // Icon colors
  Color iconIdleColor = {210, 210, 216, 255};   // Inactive soft white
  Color iconHoverColor = {255, 255, 255, 255};  // Hover pure white
  Color iconActiveColor = {215, 212, 250, 255}; // Active light lavender

  // Shortcut label colors (opacity 0.65 inactive, 0.95 active)
  Color textIdleColor = {255, 255, 255,
                         166}; // Inactive ~65% white (255 * 0.65 = 166)
  Color textHoverColor = {255, 255, 255, 204}; // Hover ~80% white
  Color textActiveColor = {255, 255, 255,
                           242}; // Active ~95% white (255 * 0.95 = 242)

  int isPressed = 0;
  bool onHover = false;
  float roundness = 0.4545f; // 10px corner radius on 44px button
  float segment = 10;
  Texture2D icon{};
  std::string shortcut;
  std::string tooltip;

  float hoverProgress = 0.0f;  // 0.0f to 1.0f (120ms transition)
  float activeProgress = 0.0f; // 0.0f to 1.0f (120ms transition)

  using BoundedShape::BoundedShape;

public:
  Button(Vector2 pos, float width = 44, float height = 48);
  void ChangeIsPressedStatus(int val);
  void Draw() const override;
  void isHovered(Vector2 mousePos);
  Vector2 GetOrigin() const;
  void setOrigin(Vector2 pos);
  float getheight() const;
  float getWitdh() const;
  void SetIcon(Texture2D iconTexture);
  void SetShortcut(const std::string &s);
  void SetTooltip(const std::string &t);
  const std::string &GetShortcut() const;
  const std::string &GetTooltip() const;
  bool IsHovered() const;
};
