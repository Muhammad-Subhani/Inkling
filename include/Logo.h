#pragma once
#include <raylib.h>

class Logo {
  Texture2D texture{};
  Font font{};
  Rectangle bounds{};
  float imageWidth = 0.0f;
  bool ownsTexture = false;
  bool ownsFont = false;

public:
  explicit Logo(Texture2D fallbackIcon);
  ~Logo();

  Logo(const Logo &) = delete;
  Logo &operator=(const Logo &) = delete;

  void Draw() const;
  bool Contains(Vector2 point) const;
  void Unload();
};
