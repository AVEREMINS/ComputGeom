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
    ready = points.size() == 4;
    if (ready) answer = geometry::intersect(points[0], points[1], points[2], points[3]);
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
    if (online && points.size() == 4) calculate();
}

QString Canvas::resultText() const
{
    if (points.size() < 4) return "Добавьте четыре точки: A, B, C, D";
    if (!ready) return "Нажмите Рассчитать";
    if (answer.type == 0) return "Пересечения нет";
    if (answer.type == 1) return QString("Пересечение: (%1; %2)").arg(answer.a.x, 0, 'f', 2).arg(answer.a.y, 0, 'f', 2);
    return QString("Общий отрезок: (%1; %2) - (%3; %4)")
        .arg(answer.a.x, 0, 'f', 2).arg(answer.a.y, 0, 'f', 2)
        .arg(answer.b.x, 0, 'f', 2).arg(answer.b.y, 0, 'f', 2);
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
    if (points.size() == 4) {
        p.setPen(QPen(QColor(220, 120, 35), 2));
        p.drawLine(geometry_qt::toScreen(points[2], size()), geometry_qt::toScreen(points[3], size()));
    }
    if (ready && answer.type == 1) {
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(210, 40, 60));
        p.drawEllipse(geometry_qt::toScreen(answer.a, size()), 7, 7);
    }
    if (ready && answer.type == 2) {
        p.setPen(QPen(QColor(150, 75, 185), 5));
        p.drawLine(geometry_qt::toScreen(answer.a, size()), geometry_qt::toScreen(answer.b, size()));
    }

    const char* names[4] = {"A", "B", "C", "D"};
    for (int i = 0; i < (int)points.size(); i++) {
        QColor color = i < 2 ? QColor(40, 105, 200) : QColor(220, 120, 35);
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
    if (points.size() < 4) {
        points.push_back(geometry_qt::fromScreen(pos, size()));
        if (points.size() == 4 && online) calculate();
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
    if (online && points.size() == 4) calculate();
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
