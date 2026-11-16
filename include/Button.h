#pragma once
#include "./BoundedShape.h"
#include <raylib.h>
class Button : public BoundedShape {
  // 35 , 35 , 41 , 255
  Color color = {35, 35, 41, 255};
  // {46 , 45 , 57 , 255 };
  Color HOverColor = {46, 45, 57, 255};
  Color SelectedCOlor = {64, 62, 106, 255};
  int isPressed = 0;
  bool onHover = 0;
  float roundness = 0.5;
  float segment = 10;
  Texture2D icon{};
  using BoundedShape::BoundedShape;

public:
  Button(Vector2 pos, float width = 40, float height = 48);
  void ChangeIsPressedStatus(int val);
  void Draw() const override;
  void isHovered(Vector2 mousePos);
  Vector2 GetOrigin() const;
  void setOrigin(Vector2 pos);
  float getheight() const;
  float getWitdh() const;
  void SetIcon(Texture2D iconTexture);
};
