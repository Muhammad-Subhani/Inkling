#pragma once
#include <raylib.h>

struct Window {
  Window(int w, int h, const char *title) { InitWindow(w, h, title); }
  ~Window() { CloseWindow(); }
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
};
