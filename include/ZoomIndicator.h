#pragma once
#include <raylib.h>

class ZoomIndicator {
  Rectangle bounds{};

public:
  void UpdateLayout(float zoom);
  void Draw(float zoom) const;
  bool Contains(Vector2 point) const;
};
