#ifndef POINT_H
#define POINT_H

#include <iostream>

namespace pt
{
    class Point
    {
        public:

            float x;
            float y;

            Point(float x1, float y1);
            Point();

            Point operator =(Point p)
            {
                if (&p != this)
                {
                    x = p.x;
                    y = p.y;
                }
                return *this;
            }
            Point operator =(float f)
            {
                x = f;
                y = f;
                return *this;
            }
            Point operator *(Point p)
            {
                if (&p != this)
                {
                    x *= p.x;
                    y *= p.y;
                }
                return *this;
            }
            Point operator *(float f)
            {
                Point p = *this;
                p.x *= f;
                p.y *= f;
                return p;
            }
            Point operator +=(Point p)
            {
                if (&p != this)
                {
                    x += p.x;
                    y += p.y;
                }
                return *this;
            }
            Point operator +=(int i)
            {
                x += i;
                y += i;
                return *this;
            }
    };

    bool operator==(Point p1, Point p2);
    bool operator<(Point p1, Point p2);
    bool operator>(Point p1, Point p2);

    Point PointMake(float x, float y);
    std::ostream& operator<< (std::ostream& out, pt::Point pot);
}
#endif
