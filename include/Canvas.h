#pragma once
#include "RectangleShape.h"
#include "Shape.h"
#include "ShapeFactory.h"
#include <algorithm>
#include <cmath>
#include <memory>
#include <raylib.h>
#include <vector>
class Canvas {
public:
  void Add(std::unique_ptr<Shape> s);
  void Draw() const;
  Shape *PickAt(Vector2 p) const;
  bool IsSelected(Shape *s) const;
  void DrawBoundaries(float zoom) const;
  void SetSelected(Vector2 p, bool SHiftLogic);
  void ClearSelected();
  Shape *BodyClickedCheck(Vector2 p) const;
  void CheckForEnclosingShapes(Rectangle temp);
  Rectangle GetWholeARect() const;
  int GetSizeOfselected() const { return selected.size(); }
  void MoveAllSelectedTogether(Vector2 p);
  std::vector<Vector2> LocationOfEachElementInbounds();
  Rectangle GetWorldBound() const;
  Rectangle GetUnionRectangleCAn(Rectangle a, Rectangle b) const;
  void DrawMiniMap() const;

private:
  std::vector<std::unique_ptr<Shape>> shapes;
  std::vector<Shape *> selected;
};
