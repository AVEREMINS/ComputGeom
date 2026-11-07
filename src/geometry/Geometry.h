#pragma once

namespace geometry {

struct Point
{
    double x = 0;
    double y = 0;
};

struct Intersection
{
    int type = 0;
    Point a;
    Point b;
};

int orientation(Point a, Point b, Point p);
bool onSegment(Point a, Point b, Point p);
Intersection intersect(Point a, Point b, Point c, Point d);

}
