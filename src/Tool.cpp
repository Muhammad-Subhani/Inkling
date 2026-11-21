#include "../include/Tool.h"
#include "../include/ShapeFactory.h"
bool ToolToShape(int tool, ShapeType &out) {
  switch (tool) {
  case TOOL_RECT:
    out = ShapeType::Rect;
    return true;
  case TOOL_ELLIPSE:
    out = ShapeType::Ellipse;
    return true;
  case TOOL_DIAMOND:
    out = ShapeType::Diamond;
    return true;
  case TOOL_SELECT:
    out = ShapeType::SelectionMarquee;
    return true;
  }

  return false; // hand, select, arrow, etc. aren't shapes yet
}
