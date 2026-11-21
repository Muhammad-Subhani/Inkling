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
#include <vector>

int main() {
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
  Vector2 pivot;
  // pivots when more than on shape is selected
  // std::vector<Vector2> allPivots;
  // this status is helping to move the slected Shape
  bool DragForMOve = false;
  Shape *Clicked;
  Vector2 prev;
  while (!WindowShouldClose()) {
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
      }
    }
    // for selecting any shape
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        MainBar.GetPressed() == TOOL_SELECT) {
      bool shift = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
      canvas.SetSelected(GetMousePosition(), shift);
    }
    // setting the pivot  the selected shape in order to move it
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        MainBar.GetPressed() == TOOL_SELECT) {
      Clicked = canvas.BodyClickedCheck(GetMousePosition());
      if (!Clicked && canvas.GetSizeOfselected() == 0) {
        pivot = {0, 0};
        prev = {0, 0};
        DragForMOve = false;
      } else if (canvas.GetSizeOfselected() == 1) {
        pivot = {GetMousePosition().x - Clicked->GetBounds().x,
                 GetMousePosition().y - Clicked->GetBounds().y};
        DragForMOve = true;
      } else if (canvas.GetSizeOfselected() > 1) {
        Rectangle temp = canvas.GetWholeARect();
        pivot = {mouse.x - temp.x, mouse.y - temp.y};
        DragForMOve = true;
      }
    }
    // moving the slected shape through dragging
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && DragForMOve) {
      Vector2 mouse = GetMousePosition();
      if (mouse.x != prev.x || mouse.y != prev.y) {
        prev = mouse;
        if (canvas.GetSizeOfselected() > 1) {
          canvas.MoveAllSelectedTogether(
              {mouse.x - pivot.x, mouse.y - pivot.y});
        } else
          Clicked->MoveElement({mouse.x - pivot.x, mouse.y - pivot.y});
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
      bool inBar =
          (mouse.x >= MainBar.GetOrigin().x &&
           mouse.x <= MainBar.GetOrigin().x + MainBar.GetDimension().x &&
           mouse.y >= MainBar.GetOrigin().y &&
           mouse.y <= MainBar.GetOrigin().y + MainBar.GetDimension().y);
      if (inBar) {
        MainBar.SelectAbutton(mouse); // click changes pressed
        canvas.ClearSelected();
        DragForMOve = false;
      } else if (MainBar.GetPressed() == TOOL_PENCIL) {
        activeStroke = std::make_unique<Stroke>();
        activeStroke->AddPoint(mouse);
      } else if (ToolToShape(MainBar.GetPressed(), current)) {
        origin = mouse; // only start a drag for shape tools
        dragging = true;
      }
    }
    // adding the stroke in the canvas array
    if (activeStroke && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      activeStroke->AddPoint(mouse);

    // mouse released
    if (MainBar.GetPressed() == TOOL_PENCIL && activeStroke &&
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (MainBar.GetPressed() == TOOL_PENCIL)
        canvas.Add(std::move(activeStroke));
    }
    if (dragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (MainBar.GetPressed() != TOOL_SELECT)
        canvas.Add(MakeShape(current, origin, mouse));
      dragging = false;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    MainBar.UpdateState(mouse);
    MainBar.Draw();
    canvas.Draw();
    canvas.DrawBoundaries();
    if (activeStroke)
      activeStroke->Draw();
    if (dragging && !DragForMOve) {
      if (MainBar.GetPressed() == TOOL_SELECT) {
        Rectangle fresh = MakeShape(current, origin, mouse)->GetBounds();
        canvas.CheckForEnclosingShapes(fresh);
      }
      MakeShape(current, origin, mouse)->Draw(); // live preview
    }
    EndDrawing();
  }

  CloseWindow();
}
