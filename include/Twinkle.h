#include <raylib.h>
#include <vector>

class Twinkle{
    public:
        void addPoint(Vector2);
        void Draw();
        void Update();
        bool notEmpty();
        void reset();
    private:
        struct PointwithTime{  Vector2 point; float time;};
        std::vector<PointwithTime> points;

};