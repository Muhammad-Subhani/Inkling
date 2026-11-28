#include "../include/Eraser.h"
#include "../include/Canvas.h"
#include <algorithm>
#include <cmath>

void Eraser::Begin(Canvas &c, Vector2 world, float zoom) {
  active = true;
  prev = world;
  Sweep(c, world, world, zoom);
}

void Eraser::Update(Canvas &c, Vector2 world, float zoom) {
  if (!active) return;
  Sweep(c, prev, world, zoom);
  prev = world;
}

// Test points along the path from a to b, not just the current mouse position.
// A fast drag moves many pixels per frame and would skip over thin strokes otherwise.
void Eraser::Sweep(Canvas &c, Vector2 a, Vector2 b, float zoom) {
  float r = RADIUS_PX / zoom;
  float dx = b.x - a.x, dy = b.y - a.y;
  float dist = std::sqrt(dx * dx + dy * dy);
  int steps = std::max(1, (int)std::ceil(dist / (r * 0.5f)));
  for (int i = 0; i <= steps; i++) {
    float t = (float)i / steps;
    Vector2 p = {a.x + dx * t, a.y + dy * t};
    //practically speaking the eraser is a circle so we need to erase at the centre 
    //but it's a pain in the ass to erase a circle so i will just erase at the centre and four points around it 
    // magar sabko lagy ga circle meh erase ho raha hai ;)
    c.EraseAt(p);                                   
    c.EraseAt({p.x + r, p.y});                      
    c.EraseAt({p.x - r, p.y});
    c.EraseAt({p.x, p.y + r});
    c.EraseAt({p.x, p.y - r});
  }
}

void Eraser::DrawCursor(Vector2 world, float zoom) const {
  DrawCircleLinesV(world, RADIUS_PX / zoom, Color{255, 255, 255, 160});
}