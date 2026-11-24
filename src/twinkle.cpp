#include "../include/Twinkle.h"
#include <algorithm>
void Twinkle::addPoint(Vector2 p){ points.push_back({p,(float)GetTime()}); }
void Twinkle::Draw(){
    static const float LIFE = 3.0f;
    for (int i = 0; i < points.size()-1; i++) //Runs when there are atleast two points availabe
    {
          float age = GetTime() - points[i].time;
        float fade = 1 - age/LIFE; //0>1
        
        DrawLineEx(points[i].point, points[i+1].point,3.0f ,Fade(RED, fade));
    }
}

void Twinkle::Update(){
    float now = (float)GetTime();

    points.erase(
        std::remove_if(points.begin(), points.end(),
            [now](const PointwithTime& p) { return (now - p.time) > 3.0f; }),
        points.end());
}

bool Twinkle::notEmpty(){
    return points.size() > 1? true : false; //Dont draw for one point
}

void Twinkle::reset(){
   points.clear();
}