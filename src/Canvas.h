#pragma once
#include <QWidget>
#include <QString>
#include <vector>
#include "geometry/Geometry.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget* parent = nullptr);
    void calculate();
    void reset();
    void setOnline(bool value);
    QString resultText() const;

signals:
    void changed();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    std::vector<geometry::Point> points;
    int dragging = -1;
    bool online = true;
    bool ready = false;
    int answer = 0;
};
