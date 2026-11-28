#pragma once
#include "Shape.h"
#include <iostream>
#include <memory>
#include <raylib.h>
enum class ShapeType { Rect, Ellipse, Diamond, Line, SelectionMarquee };
std::unique_ptr<Shape> MakeShape(ShapeType t, Vector2 a, Vector2 b);
