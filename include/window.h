#pragma once
#include <raylib.h>

struct Window {
  Window(int w, int h, const char *title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w, h, title);
    SetWindowMinSize(400, 600);
  }
  ~Window() { CloseWindow(); }
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
};
