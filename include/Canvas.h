#pragma once
#include "Shape.h"
#include <memory>
#include <raylib.h>
#include <vector>
class Canvas {
public:
  void Add(std::unique_ptr<Shape> s);
  void Draw() const;

private:
  std::vector<std::unique_ptr<Shape>> shapes;
};
