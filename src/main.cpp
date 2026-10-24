#include "Canvas.h"
#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Задача 1. Предикат поворота");
    window.resize(920, 650);

    QVBoxLayout* layout = new QVBoxLayout(&window);
    QLabel* hint = new QLabel("Кликните A, B, P. Затем перетаскивайте точки мышью.");
    layout->addWidget(hint);

    Canvas* canvas = new Canvas;
    layout->addWidget(canvas, 1);

    QHBoxLayout* buttons = new QHBoxLayout;
    QPushButton* run = new QPushButton("Рассчитать");
    QPushButton* clear = new QPushButton("Сброс");
    QCheckBox* check = new QCheckBox("Онлайн-пересчет");
    check->setChecked(true);
    buttons->addWidget(run);
    buttons->addWidget(clear);
    buttons->addWidget(check);
    layout->addLayout(buttons);

    QLabel* result = new QLabel(canvas->resultText());
    layout->addWidget(result);

    QObject::connect(run, &QPushButton::clicked, canvas, &Canvas::calculate);
    QObject::connect(clear, &QPushButton::clicked, canvas, &Canvas::reset);
    QObject::connect(check, &QCheckBox::toggled, canvas, &Canvas::setOnline);
    QObject::connect(canvas, &Canvas::changed, result, [canvas, result]() {
        result->setText(canvas->resultText());
    });

    window.show();
    return app.exec();
}
