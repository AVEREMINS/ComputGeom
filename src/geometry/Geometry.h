#pragma once
namespace geometry {
struct Point
{
    double x = 0;
    double y = 0;
};
int orientation(Point a, Point b, Point p);
int orientation(Point a, Point b, Point p, double eps);
}