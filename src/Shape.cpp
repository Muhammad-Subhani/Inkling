#include "../include/Shape.h"
int Shape::nextId = 0;
Shape::Shape() : _id(nextId++) {};
