#include "../include/Canvas.h"
#include "../include/Diamond.h"
#include "../include/Ellipse.h"
#include "../include/RectangleShape.h"
#include <memory>
#include <raylib.h>
enum class ShapeType { Rect, Ellipse, Diamond };

std::unique_ptr<BoundedShape> MakeShape(ShapeType t, Vector2 a, Vector2 b) {
  switch (t) {
  case ShapeType::Rect:
    return std::make_unique<RecatngleShape>(a, b);
  case ShapeType::Ellipse:
    return std::make_unique<Ellipse>(a, b);
  case ShapeType::Diamond:
    return std::make_unique<Diamond>(a, b);
  }
  return nullptr;
}

int main() {
  InitWindow(800, 800, "Inkling");
  SetTargetFPS(60);

  Canvas canvas;
  ShapeType current = ShapeType::Rect;
  Vector2 origin{};
  bool dragging = false;

  while (!WindowShouldClose()) {
    Vector2 mouse = GetMousePosition();

    if (IsKeyPressed(KEY_ONE))
      current = ShapeType::Rect;
    if (IsKeyPressed(KEY_TWO))
      current = ShapeType::Ellipse;
    if (IsKeyPressed(KEY_THREE))
      current = ShapeType::Diamond;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      origin = mouse;
      dragging = true;
    }
    if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      canvas.Add(MakeShape(current, origin, mouse));
      dragging = false;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    canvas.Draw();
    if (dragging)
      MakeShape(current, origin, mouse)->Draw(); // live preview
    EndDrawing();
  }
  CloseWindow();
}
