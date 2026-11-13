#include "../include/Canvas.h"
void Canvas::Add(std::unique_ptr<Shape> s) { shapes.push_back(std::move(s)); }
void Canvas::Draw() const {
  for (const auto &s : shapes)
    s->Draw();
}
