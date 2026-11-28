#pragma once
#include <raylib.h>
#include <vector>

class Twinkle{
    public:
        void addPoint(Vector2, bool);
        void Draw();
        void Update();
        bool notEmpty();
        void lineBreaker();
        void Tick(bool);
    private:
        struct PointwithTime{  Vector2 point; float time; bool valid;/*The valid identifier differ the lines*/};
        std::vector<PointwithTime> points;
        // std::vector<std::vector<PointwithTime>> lineContainer; //it stores the lines
        static constexpr float LIFE = 1.0f;
        static constexpr Color neonRed  = {255, 49, 49, 255}; 
        float twinkleClock = 0.0f;


};