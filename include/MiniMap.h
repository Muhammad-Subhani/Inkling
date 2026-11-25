#pragma once
#include "./Canvas.h"
#include "RectangleShape.h"
#include "raylib.h"
class MiniMap {
  // Actual Rectangle for minimap
  Rectangle miniWindow;
  // actuall visisble screen
  Rectangle viewport;
  // the rectangle having all the shapes drawn
  Rectangle AllShapes;
  // resulting recatngle you will see as the minimap
  Rectangle UnionWindow;
  // camera
  Camera2D miniCam;
  Color miniWindowColor = {35, 35, 41, 255};
  const Color WINDOW_FILL = {64, 62, 106, 50};

public:
  // making the window
  MiniMap(Vector2 cords, float height, float width);
  // update portion where you make the offset , target and decide the zoom
  void Update(const Canvas &canvas, const Camera2D &camera,
              Vector2 &Camera_Target);
  // this will tell either you have cliked in the minimap
  bool IsPointINMinimap(Vector2 p) const;
  void Draw(const Canvas &canvas) const;
  void SetHeightOFMiniScreen(Vector2 p);
};
