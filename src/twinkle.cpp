#include "../include/Twinkle.h"
#include <algorithm>
void Twinkle::addPoint(Vector2 p, bool state){ points.push_back({p, twinkleClock, state}); }
void Twinkle::Draw(){
    for (size_t i = 0; i + 1 < points.size(); i++) //Runs when there are atleast two points availabe, if the vector is empty> points.size() '0' unsigned, then -1 keeps to the other bound not -ve 
    {
      if (points[i].valid && points[i+1].valid)
      {
            float age = twinkleClock - points[i].time;
        float fade = std::clamp(1.0f - age/LIFE, 0.0f, 1.0f); //0>1, clamp keeps the value in range
        
        DrawLineEx(points[i].point, points[i+1].point, 3.0f, Fade(neonRed, fade));
      }
      
    }
}

void Twinkle::Update(){
    float now = twinkleClock;

    points.erase(
        std::remove_if(points.begin(), points.end(),//.end() is the size+1
            [now](const PointwithTime& p) { return ((now - p.time) > LIFE); }),
        points.end());
}

bool Twinkle::notEmpty(){
    return points.size() > 1? true : false; //Dont draw for one point
}

// 

void Twinkle::lineBreaker(){
    addPoint({0,0},false);
}

void Twinkle::Tick(bool drawing){
    if (!drawing)
    {
        twinkleClock += GetFrameTime();
    }
    
}