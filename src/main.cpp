#include "../include/Bar.h"
#include "../include/Button.h"
#include "../include/Canvas.h"
#include "../include/Icons.h"
#include "../include/Logo.h"
#include "../include/MiniMap.h"
#include "../include/ShapeFactory.h"
#include "../include/Stroke.h"
#include "../include/Tool.h"
#include "../include/ZoomIndicator.h"
#include "../include/window.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <raylib.h>
#include <vector>
Rectangle GetUnionRectangle(Rectangle a, Rectangle b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y),
          std::max(a.x + a.width, b.x + b.width) - std::min(a.x, b.x),
          std::max(a.y + a.height, b.y + b.height) - std::min(a.y, b.y)};
}
bool IsPointOverUI(Vector2 point, const Bar &bar, const MiniMap &miniMap,
                   const Logo &logo, const ZoomIndicator &zoomIndicator) {
  Rectangle barRect = {bar.GetOrigin().x, bar.GetOrigin().y,
                       bar.GetDimension().x, bar.GetDimension().y};
  return CheckCollisionPointRec(point, barRect) ||
         miniMap.IsPointINMinimap(point) || logo.Contains(point) ||
         zoomIndicator.Contains(point);
}
int main() {
  Color windowColor = {18, 18, 18, 255};
  Window window(800, 800, "Inkling");
  SetTargetFPS(60);
  Icons icons({"assets/icons/hand.png", "assets/icons/select.png",
               "assets/icons/rectangle.png", "assets/icons/diamond.png",
               "assets/icons/ellipse.png", "assets/icons/arrow.png",
               "assets/icons/line.png", "assets/icons/pencil.png",
               "assets/icons/text.png", "assets/icons/twinkle.png",
               "assets/icons/eraser.png"});
  const std::vector<const char *> shortcuts = {"H", "V", "R", "D", "O", "A",
                                               "L", "P", "T", "K", "E"};
  const std::vector<const char *> tooltips = {
      "Hand - H",    "Select - V",     "Rectangle - R", "Diamond - D",
      "Ellipse - O", "Arrow - A",      "Line - L",      "Draw - P",
      "Text - T",    "Magic wand - K", "Eraser - E"};

  Canvas canvas;
  Vector2 Camera_Target = {0, 0};
  Camera2D camera{
      {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, Camera_Target, 0, 1};
  float MIN_ZOOM = 0.52f;
  float MAX_ZOOM = 10.0f;
  ShapeType current = ShapeType::Rect;
  Vector2 origin{};
  bool dragging = false;
  float MainBarX = ToolbarConfig::TOOLBAR_LEFT;
  float canvasTop = ToolbarConfig::TOP_BAR_HEIGHT;
  float canvasHeight = (float)GetScreenHeight() - canvasTop;
  float MainBarY = canvasTop + (canvasHeight - 560.0f) / 2.0f;
  Bar MainBar(Vector2{MainBarX, MainBarY});
  for (size_t i = 0; i < 11; i++) {
    auto btn =
        std::make_unique<Button>(Vector2{0, 0}, ToolbarConfig::TOOL_BTN_SIZE,
                                 ToolbarConfig::TOOL_BTN_SIZE);
    if (i < icons.Count()) {
      btn->SetIcon(icons.Get(i));
    }
    if (i < shortcuts.size()) {
      btn->SetShortcut(shortcuts[i]);
    }
    if (i < tooltips.size()) {
      btn->SetTooltip(tooltips[i]);
    }
    MainBar.Add(std::move(btn));
  }
  // Center toolbar within visible canvas area (top: 50%; transform:
  // translateY(-50%), adjusted for any top bar)
  MainBarY = canvasTop + (canvasHeight - MainBar.GetDimension().y) / 2.0f;
  MainBar.SetPosition(Vector2{MainBarX, MainBarY});

  MainBar.ChangePressed(2); // rectangle highlighted by default
  std::unique_ptr<Stroke> activeStroke;
  Vector2 pivot{};
  // pivots when more than on shape is selected
  // std::vector<Vector2> allPivots;
  // this status is helping to move the slected Shape
  bool DragForMOve = false;
  Shape *Clicked = nullptr;
  Vector2 prev{};
  float Zoom = 1;
  Vector2 Pivot_Canvas = {0, 0};
  float MiniMap_height = 200.0;
  float MiniMap_width = 250.0;
  MiniMap MiniMap(
      {(float)GetScreenWidth() - MiniMap_width - ToolbarConfig::TOOLBAR_LEFT,
       ToolbarConfig::TOOLBAR_LEFT},
      MiniMap_height, MiniMap_width);
  Logo logo(icons.Get(TOOL_PENCIL));
  ZoomIndicator zoomIndicator;
  zoomIndicator.UpdateLayout(Zoom);
  while (!WindowShouldClose()) {

    // this part of code checks the zoom if applied else remains in 0.52--10
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
      float MouseWheelTicks = GetMouseWheelMove();
      Zoom *= std::pow(1.1f, MouseWheelTicks);
      Zoom = std::clamp(Zoom, MIN_ZOOM, MAX_ZOOM);
    }
    camera.zoom = Zoom;
    // moving vertically
    if (!IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL)) {
      if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
        float MouseWheelTicks = GetMouseWheelMove();
        Camera_Target = {Camera_Target.x + (100 * MouseWheelTicks),
                         Camera_Target.y};
      } else if (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT)) {
        float MouseWheelTicks = GetMouseWheelMove();
        Camera_Target = {Camera_Target.x,
                         Camera_Target.y + (100 * MouseWheelTicks * -1)};
      }
    }
    MiniMap.SetHeightOFMiniScreen(
        {(float)GetScreenWidth() - MiniMap_width - ToolbarConfig::TOOLBAR_LEFT,
         ToolbarConfig::TOOLBAR_LEFT});
    MiniMap.Update(canvas, camera, Camera_Target);
    zoomIndicator.UpdateLayout(Zoom);

    // Keep toolbar vertically centered in visible canvas area when window is
    // resized
    float currentCanvasTop = ToolbarConfig::TOP_BAR_HEIGHT;
    float currentCanvasHeight = (float)GetScreenHeight() - currentCanvasTop;
    float targetBarY = currentCanvasTop +
                       (currentCanvasHeight - MainBar.GetDimension().y) / 2.0f;
    if (MainBar.GetOrigin().y != targetBarY ||
        MainBar.GetOrigin().x != ToolbarConfig::TOOLBAR_LEFT) {
      MainBar.SetPosition(Vector2{ToolbarConfig::TOOLBAR_LEFT, targetBarY});
    }
    // this is the mouse for the screen
    Vector2 mouse = GetMousePosition();
    // this is the mouse coords needed for the actual infiinte world
    Vector2 WorldMouse = GetScreenToWorld2D(mouse, camera);
    if (!dragging) { // don't switch tools mid-drag
      if (IsKeyPressed(KEY_H))
        MainBar.ChangePressed(TOOL_HAND);
      if (IsKeyPressed(KEY_V))
        MainBar.ChangePressed(TOOL_SELECT);
      if (IsKeyPressed(KEY_R))
        MainBar.ChangePressed(TOOL_RECT);
      if (IsKeyPressed(KEY_D))
        MainBar.ChangePressed(TOOL_DIAMOND);
      if (IsKeyPressed(KEY_O))
        MainBar.ChangePressed(TOOL_ELLIPSE);
      if (IsKeyPressed(KEY_A))
        MainBar.ChangePressed(TOOL_ARROW);
      if (IsKeyPressed(KEY_L))
        MainBar.ChangePressed(TOOL_LINE);
      if (IsKeyPressed(KEY_P))
        MainBar.ChangePressed(TOOL_PENCIL);
      if (IsKeyPressed(KEY_T))
        MainBar.ChangePressed(TOOL_TEXT);
      if (IsKeyPressed(KEY_H) || IsKeyPressed(KEY_V) || IsKeyPressed(KEY_R) ||
          IsKeyPressed(KEY_D) || IsKeyPressed(KEY_O) || IsKeyPressed(KEY_A) ||
          IsKeyPressed(KEY_L) || IsKeyPressed(KEY_P) || IsKeyPressed(KEY_T)) {
        canvas.ClearSelected();
        DragForMOve = false;
        Pivot_Canvas = mouse;
        if (activeStroke)
          canvas.Add(std::move(activeStroke));
      }
    }

    // checks three things
    // if the click is under the bar it means youve tried to change the tool
    // you are trying to draw a thing so set the origin
    // you are using the pencil .. so make the empty stroke object
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      // the element which is clicked just now is it already selected or not ?
      // remeber the distinction
      // if there is a click on the bar its the status change
      // when not then it means the things getting drawn
      if (IsPointOverUI(mouse, MainBar, MiniMap, logo, zoomIndicator)) {
        Rectangle barRect = {MainBar.GetOrigin().x, MainBar.GetOrigin().y,
                             MainBar.GetDimension().x,
                             MainBar.GetDimension().y};
        if (CheckCollisionPointRec(mouse, barRect)) {
          MainBar.SelectAbutton(mouse); // click changes pressed
          canvas.ClearSelected();
          DragForMOve = false;
          Clicked = nullptr;
        }
      } else {
        if (MainBar.GetPressed() == TOOL_HAND) {
          Pivot_Canvas = mouse;
        }
        // this is a case for pencil stroke
        if (MainBar.GetPressed() == TOOL_PENCIL) {
          activeStroke = std::make_unique<Stroke>();
          activeStroke->AddPoint(WorldMouse);
        }
        // this piece of code decides which element will be drawn
        if (ToolToShape(MainBar.GetPressed(), current)) {
          origin = WorldMouse; // only start a drag for shape tools
          dragging = true;
        }
        // for selecting any shape
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            MainBar.GetPressed() == TOOL_SELECT) {
          bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
          canvas.SetSelected(WorldMouse, shift);
          if (canvas.GetSizeOfselected() == 0) {
            Clicked = nullptr;
            DragForMOve = false;
          }
        }
        // setting the pivot  the selected shape in order to move it
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            MainBar.GetPressed() == TOOL_SELECT) {
          Clicked = canvas.BodyClickedCheck(WorldMouse);
          if (!Clicked && canvas.GetSizeOfselected() == 0) {
            pivot = {0, 0};
            prev = {0, 0};
            DragForMOve = false;
            Clicked = nullptr;
          } else if (Clicked && canvas.GetSizeOfselected() == 1) {
            pivot = {WorldMouse.x - Clicked->GetBounds().x,
                     WorldMouse.y - Clicked->GetBounds().y};
            DragForMOve = true;
          } else if (canvas.GetSizeOfselected() > 1) {
            Rectangle temp = canvas.GetWholeARect();
            pivot = {WorldMouse.x - temp.x, WorldMouse.y - temp.y};
            DragForMOve = true;
          }
        }
      }
    }

    // this portion when the button is being pressed and holded
    // adding the stroke in the canvas array
    if (activeStroke && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      activeStroke->AddPoint(WorldMouse);
    // moving the slected shape through dragging
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && DragForMOve) {
      // Vector2 mouse = GetMousePosition();
      if (WorldMouse.x != prev.x || WorldMouse.y != prev.y) {
        prev = WorldMouse;
        if (canvas.GetSizeOfselected() > 1) {
          canvas.MoveAllSelectedTogether(
              {WorldMouse.x - pivot.x, WorldMouse.y - pivot.y});
        } else
          Clicked->MoveElement(
              {WorldMouse.x - pivot.x, WorldMouse.y - pivot.y});
      }
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
        MainBar.GetPressed() == TOOL_HAND) {
      float Xpos = mouse.x - Pivot_Canvas.x;
      float Ypos = mouse.y - Pivot_Canvas.y;
      Camera_Target = {Camera_Target.x - Xpos, Camera_Target.y - Ypos};
      Pivot_Canvas = mouse;
    }
    // mouse released
    if (MainBar.GetPressed() == TOOL_PENCIL && activeStroke &&
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (MainBar.GetPressed() == TOOL_PENCIL)
        canvas.Add(std::move(activeStroke));
    }
    if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (MainBar.GetPressed() != TOOL_SELECT) {
        std::cout << " ORGIIN " << origin.x << " and y " << origin.y
                  << std::endl;
        canvas.Add(MakeShape(current, origin, WorldMouse));
      }
      dragging = false;
    }
    camera.target = Camera_Target;

    BeginDrawing();
    ClearBackground(windowColor);
    BeginMode2D(camera);
    canvas.Draw();
    canvas.DrawBoundaries(Zoom);
    if (activeStroke)
      activeStroke->Draw();
    if (dragging && !DragForMOve) {
      if (MainBar.GetPressed() == TOOL_SELECT) {
        Rectangle fresh = MakeShape(current, origin, WorldMouse)->GetBounds();
        canvas.CheckForEnclosingShapes(fresh);
      }
      MakeShape(current, origin, WorldMouse)->Draw(); // live preview
    }
    EndMode2D();

    MainBar.UpdateState(mouse);
    MainBar.Draw();
    logo.Draw();
    MiniMap.Draw(canvas);
    zoomIndicator.Draw(Zoom);
    EndDrawing();
  }

  logo.Unload();
  CloseWindow();
}
