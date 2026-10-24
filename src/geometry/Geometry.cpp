#include "geometry/Geometry.h"
#include <cmath>

namespace geometry {
int orientation(Point a, Point b, Point p, double eps)
{
    double dx = b.x-a.x;
    double dy = b.y-a.y;
    double s = dx*(p.y-a.y)-dy*(p.x-a.x);
    double len = std::sqrt(dx*dx+dy*dy);
    if (len < 1e-9) return 0;
    if (std::abs(s) <= eps*len) return 0;
    if (s > 0) return 1;
    return -1;
}
int orientation(Point a, Point b, Point p)
{
    return orientation(a, b, p, 1e-9);
}
}