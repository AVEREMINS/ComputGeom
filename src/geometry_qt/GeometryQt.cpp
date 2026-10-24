#include "geometry_qt/GeometryQt.h"

namespace geometry_qt {

geometry::Point fromScreen(QPointF pos, QSize size)
{
    return {pos.x()-size.width()/2.0, size.height()/2.0-pos.y()};
}

QPointF toScreen(geometry::Point point, QSize size)
{
    return QPointF(point.x+size.width()/2.0, size.height()/2.0-point.y);
}

}
