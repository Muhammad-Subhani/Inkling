#pragma once
#include "Shape.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <cstddef>

class TextShape : public Shape
{
public:
  explicit TextShape(Vector2 pos, float size = 24) : pos_(pos), size_(size) {}

  void Draw() const override
  {
    DrawText(text_.c_str(), (int)pos_.x, (int)pos_.y, (int)size_, color);
    if (editing_ && std::fmod(GetTime(), 1.0) < 0.5)
    {
      int w = MeasureText(text_.substr(0, cursor_).c_str(), (int)size_);
      DrawRectangle((int)pos_.x + w + 2, (int)pos_.y, 2, (int)size_, color);
    }
  }

  void HandleTyping()
  {
    int c;
    while ((c = GetCharPressed()) != 0)
      if (c >= 32 && c <= 126)
      {
        text_.insert(cursor_, 1, (char)c);
        cursor_++;
      }

    if (Pressed(KEY_BACKSPACE) && cursor_ > 0)
    {
      text_.erase(cursor_ - 1, 1);
      cursor_--;
    }
    if (Pressed(KEY_DELETE) && cursor_ < text_.size())
      text_.erase(cursor_, 1);
    if (Pressed(KEY_LEFT) && cursor_ > 0)
      cursor_--;
    if (Pressed(KEY_RIGHT) && cursor_ < text_.size())
      cursor_++;
    if (IsKeyPressed(KEY_HOME))
      cursor_ = 0;
    if (IsKeyPressed(KEY_END))
      cursor_ = text_.size();
  }

  // put the cursor at the character boundary closest to the click
  void SetCursorFromClick(Vector2 p)
  {
    float x = p.x - pos_.x;
    size_t best = 0;
    float bestDist = std::fabs(x);
    for (size_t i = 1; i <= text_.size(); i++)
    {
      float d = std::fabs(x - MeasureText(text_.substr(0, i).c_str(), (int)size_));
      if (d < bestDist)
      {
        bestDist = d;
        best = i;
      }
    }
    cursor_ = best;
  }

  Rectangle GetBounds() const override
  {
    int w = MeasureText(text_.c_str(), (int)size_);
    return {pos_.x, pos_.y, (float)std::max(w, 10), size_};
  }
  bool Contains(Vector2 p) const override
  {
    return CheckCollisionPointRec(p, GetBounds());
  }
  bool ClickedInBody(Vector2 p) const override
  {
    return CheckCollisionPointRec(p, GetBounds());
  }
  void MoveElement(Vector2 topLeft) override { pos_ = topLeft; }

  bool Empty() const { return text_.empty(); }
  void SetEditing(bool e) { editing_ = e; }

private:
  static bool Pressed(int k) { return IsKeyPressed(k) || IsKeyPressedRepeat(k); }
  size_t cursor_ = 0;
  std::string text_;
  Vector2 pos_;
  float size_;
  bool editing_ = true;
};