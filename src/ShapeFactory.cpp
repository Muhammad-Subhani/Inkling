#include "../include/ShapeFactory.h"
#include "../include/Diamond.h"
#include "../include/Ellipse.h"
#include "../include/RectangleShape.h"
#include "../include/SelectionMarquee.h"
std::unique_ptr<BoundedShape> MakeShape(ShapeType t, Vector2 a, Vector2 b) {
  switch (t) {
  case ShapeType::Rect:
    return std::make_unique<RecatngleShape>(a, b);
  case ShapeType::Ellipse:
    return std::make_unique<Ellipse>(a, b);
  case ShapeType::Diamond:
    return std::make_unique<Diamond>(a, b);
  case ShapeType::SelectionMarquee:
    return std::make_unique<SelectionMarquee>(a, b);
  }

  return nullptr;
}
