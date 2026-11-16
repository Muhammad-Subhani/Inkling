#pragma once
#include "BoundedShape.h"
#include <iostream>
#include <memory>
#include <raylib.h>
enum class ShapeType { Rect, Ellipse, Diamond };
std::unique_ptr<BoundedShape> MakeShape(ShapeType t, Vector2 a, Vector2 b);
