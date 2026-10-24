#pragma once
#include "geometry/Geometry.h"
#include <QPointF>
#include <QSize>

namespace geometry_qt {

geometry::Point fromScreen(QPointF pos, QSize size);
QPointF toScreen(geometry::Point point, QSize size);

}
