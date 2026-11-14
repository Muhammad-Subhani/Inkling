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
  void LayoutUpdate();

public:
  Bar(Vector2 pos, float width = 60, float height = 0);
  void Draw() const override;
  void Add(std::unique_ptr<Button> S);
  void UpdateState(Vector2 mouse);
};
