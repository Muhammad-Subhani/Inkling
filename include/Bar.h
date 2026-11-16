#pragma once
#include "./BoundedShape.h"
#include "./Button.h"
#include <memory>
#include <raylib.h>
#include <vector>
class Bar : public BoundedShape {
  Color barcolor = {35, 35, 41, 255};
  std::vector<std::unique_ptr<Button>> allButtons;
  float roundness = 0.5;
  float segment = 10;
  int pressed = 2;
  void LayoutUpdate();
  void SetWhichButtonPressed();

public:
  Bar(Vector2 pos, float width = 60, float height = 0);
  void Draw() const override;
  void Add(std::unique_ptr<Button> S);
  void UpdateState(Vector2 mouse);
  void SelectAbutton(Vector2 mouse);
  void ChangePressed(int val) {
    pressed = val;
    SetWhichButtonPressed();
  };
  Vector2 GetOrigin() const;
  Vector2 GetDimension() const;
  int GetPressed() const { return pressed; }
};
