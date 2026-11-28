#pragma once
#include "Shape.h"
#include <algorithm>
#include <cmath>
#include <string>

class TextShape : public Shape {
public:
  explicit TextShape(Vector2 pos, float size = 24) : pos_(pos), size_(size) {}

  void Draw() const override {
    DrawText(text_.c_str(), (int)pos_.x, (int)pos_.y, (int)size_, color);
    if (editing_ && std::fmod(GetTime(), 1.0) < 0.5) { // blinking caret
      int w = MeasureText(text_.c_str(), (int)size_);
      DrawRectangle((int)pos_.x + w + 2, (int)pos_.y, 2, (int)size_, color);
    }
  }
  Rectangle GetBounds() const override {
    int w = MeasureText(text_.c_str(), (int)size_);
    return {pos_.x, pos_.y, (float)std::max(w, 10), size_};
  }
  bool Contains(Vector2 p) const override {
    return CheckCollisionPointRec(p, GetBounds());
  }
  bool ClickedInBody(Vector2 p) const override {
    return CheckCollisionPointRec(p, GetBounds());
  }
  void MoveElement(Vector2 topLeft) override { pos_ = topLeft; }

  void HandleTyping() {
    int c;
    while ((c = GetCharPressed()) != 0)
      if (c >= 32 && c <= 126) text_ += (char)c; // ASCII only for now
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE))
      if (!text_.empty()) text_.pop_back();
  }
  bool Empty() const { return text_.empty(); }
  void SetEditing(bool e) { editing_ = e; }

private:
  std::string text_;
  Vector2 pos_;
  float size_;
  bool editing_ = true;
  
};