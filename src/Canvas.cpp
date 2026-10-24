#include "Canvas.h"
#include "geometry_qt/GeometryQt.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

Canvas::Canvas(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(450, 320);
    setMouseTracking(true);
}


void Canvas::calculate()
{
    ready = points.size() == 3;
    if (ready) answer = geometry::orientation(points[0], points[1], points[2], 5.0);
    emit changed();
    update();
}


void Canvas::reset()
{
    points.clear();
    dragging = -1;
    ready = false;
    emit changed();
    update();
}

void Canvas::setOnline(bool value)
{
    online = value;
    if (online && points.size() == 3) calculate();
}

QString Canvas::resultText() const
{
    if (points.size() < 3) return "Добавьте три точки: A, B, P";
    if (!ready) return "Нажмите Рассчитать";
    if (answer == 1) return "1: P слева от AB";
    if (answer == -1) return "-1: P справа от AB";
    return "0: P на прямой AB";
}

void Canvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::white);
    p.setPen(QPen(QColor(225, 225, 225), 1));
    for (int x = 0; x < width(); x += 25) p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 25) p.drawLine(0, y, width(), y);

    if (points.size() >= 2) {
        p.setPen(QPen(QColor(40, 105, 200), 2));
        p.drawLine(geometry_qt::toScreen(points[0], size()), geometry_qt::toScreen(points[1], size()));
    }

    const char* names[3] = {"A", "B", "P"};
    for (int i = 0; i < (int)points.size(); i++) {
        QColor color(40, 105, 200);
        if (i == 2) {
            color = QColor(90, 90, 90);
            if (ready && answer == 1) color = QColor(20, 150, 80);
            if (ready && answer == -1) color = QColor(205, 50, 60);
            if (ready && answer == 0) color = QColor(150, 90, 180);
        }
        QPointF pos = geometry_qt::toScreen(points[i], size());
        p.setPen(QPen(color, 1));
        p.setBrush(color);
        p.drawEllipse(pos, 5, 5);
        p.setPen(Qt::black);
        p.drawText(pos+QPointF(8, -8), names[i]);
    }
}

void Canvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    QPointF pos = event->position();
    for (int i = 0; i < (int)points.size(); i++) {
        QPointF p = geometry_qt::toScreen(points[i], size());
        double dx = p.x()-pos.x();
        double dy = p.y()-pos.y();
        if (dx*dx+dy*dy <= 144) {
            dragging = i;
            return;
        }
    }
    if (points.size() < 3) {
        points.push_back(geometry_qt::fromScreen(pos, size()));
        if (points.size() == 3 && online) calculate();
        else {
            ready = false;
            emit changed();
            update();
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging < 0 || !(event->buttons() & Qt::LeftButton)) return;
    points[dragging] = geometry_qt::fromScreen(event->position(), size());
    if (online && points.size() == 3) calculate();
    else {
        ready = false;
        emit changed();
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent*)
{
    dragging = -1;
}
