#pragma once
#include "ShapeFactory.h"
enum Tool {
  TOOL_HAND,
  TOOL_SELECT,
  TOOL_RECT,
  TOOL_DIAMOND,
  TOOL_ELLIPSE,
  TOOL_ARROW,
  TOOL_LINE,
  TOOL_PENCIL,
  TOOL_TEXT,
  TOOL_TWINKLE,
  TOOL_ERASER
};

bool ToolToShape(int tool, ShapeType &out);
