#include "../include/Bar.h"
#include "../include/Button.h"
#include "../include/Canvas.h"
#include "../include/Icons.h"
#include "../include/ShapeFactory.h"
#include "../include/Stroke.h"
#include "../include/Tool.h"
#include "../include/window.h"
#include <cstddef>
#include <memory>
#include <raylib.h>

int main() {
  Window window(800, 800, "Inkling");
  SetTargetFPS(60);

  Icons icons({"assets/icons/hand.png", "assets/icons/select.png",
               "assets/icons/rectangle.png", "assets/icons/diamond.png",
               "assets/icons/ellipse.png", "assets/icons/arrow.png",
               "assets/icons/line.png", "assets/icons/pencil.png",
               "assets/icons/text.png", "assets/icons/twinkle.png",
               "assets/icons/eraser.png"});

  Canvas canvas;
  ShapeType current = ShapeType::Rect;
  Vector2 origin{};
  bool dragging = false;
  float MainBarX = 20;
  float MainBarY = (GetScreenHeight() - 600) / 2.0;
  Bar MainBar(Vector2{MainBarX, MainBarY});
  for (size_t i = 0; i < 11; i++) {
    auto btn = std::make_unique<Button>(Vector2{0, 0});
    if (i < icons.Count()) {
      btn->SetIcon(icons.Get(i));
    }
    MainBar.Add(std::move(btn));
  }
  MainBar.ChangePressed(2); // rectangle highlighted by default
  std::unique_ptr<Stroke> activeStroke;
  while (!WindowShouldClose()) {
    Vector2 mouse = GetMousePosition();
    if (!dragging) { // don't switch tools mid-drag
      if (IsKeyPressed(KEY_H))
        MainBar.ChangePressed(TOOL_HAND);
      if (IsKeyPressed(KEY_V))
        MainBar.ChangePressed(TOOL_SELECT);
      if (IsKeyPressed(KEY_R))
        MainBar.ChangePressed(TOOL_RECT);
      if (IsKeyPressed(KEY_D))
        MainBar.ChangePressed(TOOL_DIAMOND);
      if (IsKeyPressed(KEY_E))
        MainBar.ChangePressed(TOOL_ELLIPSE);
      if (IsKeyPressed(KEY_A))
        MainBar.ChangePressed(TOOL_ARROW);
      if (IsKeyPressed(KEY_L))
        MainBar.ChangePressed(TOOL_LINE);
      if (IsKeyPressed(KEY_P))
        MainBar.ChangePressed(TOOL_PENCIL);
      if (IsKeyPressed(KEY_T))
        MainBar.ChangePressed(TOOL_TEXT);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      // remeber the distinction
      // if there is a click on the bar its the status change
      // when not then it means the things getting drawn
      bool inBar =
          (mouse.x >= 0 &&
           mouse.x <= MainBar.GetOrigin().x + MainBar.GetDimension().x &&
           mouse.y >= MainBar.GetOrigin().y &&
           mouse.y <= MainBar.GetOrigin().y + MainBar.GetDimension().y);
      if (inBar) {
        MainBar.SelectAbutton(mouse); // click changes pressed
      } else if (MainBar.GetPressed() == TOOL_PENCIL) {
        activeStroke = std::make_unique<Stroke>();
        activeStroke->AddPoint(mouse);
      } else if (ToolToShape(MainBar.GetPressed(), current)) {
        origin = mouse; // only start a drag for shape tools
        dragging = true;
      }
    }
    if (activeStroke && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      activeStroke->AddPoint(mouse);

    // mouse released
    if (MainBar.GetPressed() == TOOL_PENCIL && activeStroke &&
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (MainBar.GetPressed() == TOOL_PENCIL)
        canvas.Add(std::move(activeStroke));
    }
    if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      canvas.Add(MakeShape(current, origin, mouse));
      dragging = false;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    MainBar.UpdateState(mouse);
    MainBar.Draw();
    canvas.Draw();
    if (activeStroke)
      activeStroke->Draw();
    if (dragging)
      MakeShape(current, origin, mouse)->Draw(); // live preview
    EndDrawing();
  }

  CloseWindow();
}
