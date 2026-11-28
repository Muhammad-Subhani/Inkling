#pragma once
#include <raylib.h>

class Canvas;

class Eraser {
public:
  void Begin(Canvas &c, Vector2 world, float zoom);
  void Update(Canvas &c, Vector2 world, float zoom);
  void End() { active = false; }
  void DrawCursor(Vector2 world, float zoom) const;

private:
  void Sweep(Canvas &c, Vector2 a, Vector2 b, float zoom);
  static constexpr float RADIUS_PX = 8.0f;   // screen pixels, divided by zoom for world units
  Vector2 prev{};
  bool active = false;
};