#include "../include/MiniMap.h"
#include "../include/Canvas.h"
#include <algorithm>
#include <raylib.h>
MiniMap::MiniMap(Vector2 cords, float height, float width) {
  miniWindow = {cords.x, cords.y, width, height};
}
void MiniMap::Update(const Canvas &canvas, const Camera2D &camera,
                     Vector2 &Camera_Target) {
  // first find the the screen you are viewing
  Vector2 TopLeft = GetScreenToWorld2D({0, 0}, camera);
  Vector2 BottomRIght = GetScreenToWorld2D(
      {(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);
  viewport = {TopLeft.x, TopLeft.y, BottomRIght.x - TopLeft.x,
              BottomRIght.y - TopLeft.y};
  // now the rectangle conating all the shapes
  AllShapes = canvas.GetWorldBound();
  // this is the final rectangle that represent the one that has fitted the view
  // Screen + the all the shapes
  UnionWindow = canvas.GetUnionRectangleCAn(viewport, AllShapes);
  // setting the target which is the half of the unionRectangle

  // calculations needed to make a camera that will fit the union rectangle we
  // just made
  Vector2 minioffset = {(2 * miniWindow.x + miniWindow.width) / 2,
                        (2 * miniWindow.y + miniWindow.height) / 2};
  Vector2 miniTarget = {(2 * UnionWindow.x + UnionWindow.width) / 2,
                        (2 * UnionWindow.y + UnionWindow.height) / 2};
  float Zoom = std::min(miniWindow.width / UnionWindow.width,
                        miniWindow.height / UnionWindow.height);
  miniCam.offset = minioffset;
  miniCam.target = miniTarget;
  miniCam.zoom = Zoom;
  miniCam.rotation = 0;
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      IsPointINMinimap(GetMousePosition())) {
    Camera_Target = GetScreenToWorld2D(GetMousePosition(), miniCam);
  }
}
bool MiniMap::IsPointINMinimap(Vector2 p) const {
  return CheckCollisionPointRec(p, miniWindow);
}
void MiniMap::Draw(const Canvas &canvas) const {

  DrawRectangleRounded(miniWindow, 0.2, 8, miniWindowColor);
  BeginScissorMode((int)miniWindow.x, (int)miniWindow.y, (int)miniWindow.width,
                   (int)miniWindow.height);

  BeginMode2D(miniCam);
  DrawRectangleRounded(UnionWindow, 0.2, 8, miniWindowColor);
  // all the drawings must be representde in the for of white rectangle
  canvas.DrawMiniMap();
  // the purple vieport window
  DrawRectangleRec(viewport, WINDOW_FILL);
  EndMode2D();
  EndScissorMode();
}
void MiniMap::SetHeightOFMiniScreen(Vector2 p) {
  miniWindow.x = p.x;
  miniWindow.y = p.y;
}
