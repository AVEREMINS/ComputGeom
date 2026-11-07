#include "geometry/Geometry.h"
#include <algorithm>
#include <cmath>
#include <vector>

namespace geometry {

const double eps = 1e-9;

double cross(Point a, Point b)
{
    return a.x*b.y-a.y*b.x;
}

Point sub(Point a, Point b)
{
    return {a.x-b.x, a.y-b.y};
}

int orientation(Point a, Point b, Point p)
{
    double s = cross(sub(b, a), sub(p, a));
    if (s > eps) return 1;
    if (s < -eps) return -1;
    return 0;
}

bool onSegment(Point a, Point b, Point p)
{
    if (orientation(a, b, p) != 0) return false;
    return p.x >= std::min(a.x,b.x)-eps && p.x <= std::max(a.x,b.x)+eps &&
           p.y >= std::min(a.y,b.y)-eps && p.y <= std::max(a.y,b.y)+eps;
}

bool samePoint(Point a, Point b)
{
    return std::abs(a.x-b.x) < eps && std::abs(a.y-b.y) < eps;
}

Intersection intersect(Point a, Point b, Point c, Point d)
{
    Intersection ans;
    Point r = sub(b, a);
    Point s = sub(d, c);
    double len1 = r.x*r.x+r.y*r.y;
    double len2 = s.x*s.x+s.y*s.y;

    if (len1 < eps*eps && len2 < eps*eps) {
        if (samePoint(a, c)) return {1, a, a};
        return ans;
    }
    if (len1 < eps*eps) {
        if (onSegment(c, d, a)) return {1, a, a};
        return ans;
    }
    if (len2 < eps*eps) {
        if (onSegment(a, b, c)) return {1, c, c};
        return ans;
    }

    double det = cross(r, s);
    Point ac = sub(c, a);

    if (std::abs(det) > eps) {
        double t = cross(ac, s)/det;
        double u = cross(ac, r)/det;
        if (t >= -eps && t <= 1+eps && u >= -eps && u <= 1+eps) {
            t = std::max(0.0, std::min(1.0, t));
            Point p{a.x+t*r.x, a.y+t*r.y};
            return {1, p, p};
        }
        return ans;
    }

    if (orientation(a, b, c) != 0) return ans;

    std::vector<Point> points;
    Point ends[4] = {a, b, c, d};
    for (Point p : ends) {
        if (onSegment(a, b, p) && onSegment(c, d, p)) {
            bool exists = false;
            for (Point q : points) {
                if (samePoint(p, q)) exists = true;
            }
            if (!exists) points.push_back(p);
        }
    }
    if (points.empty()) return ans;
    if (points.size() == 1) return {1, points[0], points[0]};

    if (std::abs(r.x) >= std::abs(r.y)) {
        std::sort(points.begin(), points.end(), [](Point p, Point q) { return p.x < q.x; });
    } else {
        std::sort(points.begin(), points.end(), [](Point p, Point q) { return p.y < q.y; });
    }
    return {2, points.front(), points.back()};
}

}
