#pragma once
#include "Shape.h"
#include <memory>
#include <raylib.h>
#include <vector>
class Canvas {
public:
  void Add(std::unique_ptr<Shape> s);
  void Draw() const;
  Shape *PickAt(Vector2 p) const;
  bool IsSelected(Shape *s) const;
  void DrawBoundaries() const;
  void SetSelected(Vector2 p, bool SHiftLogic);
  void ClearSelected();
  Shape *BodyClickedCheck(Vector2 p) const;

private:
  std::vector<std::unique_ptr<Shape>> shapes;
  std::vector<Shape *> selected;
};
