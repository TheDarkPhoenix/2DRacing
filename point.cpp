#include "point.h"

namespace pt
{
    Point PointMake(float x, float y)
    {
        Point p(x, y);
        return p;
    }

    Point::Point(float x1, float y1)
    {
        x = x1;
        y = y1;
    }

    Point::Point()
    {
        x = 0;
        y = 0;
    }

    std::ostream& operator<< (std::ostream& out, pt::Point pot)
    {
        out << pot.x << ' ' << pot.y;
        return out;
    }

    bool operator==(Point p1, Point p2)
    {
        if (p1.x == p2.x && p1.y == p2.y)
            return true;
        else
            return false;
    }

    bool operator<(Point p1, Point p2)
    {
        if (p1.x < p2.x && p1.y < p2.y)
            return true;
        else
            return false;
    }

    bool operator>(Point p1, Point p2)
    {
        if (p1.x > p2.x && p1.y > p2.y)
            return true;
        else
            return false;
    }
}
