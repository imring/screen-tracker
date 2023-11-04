#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QApplication>
#include <QImageReader>

#include <iostream>

#include "tracker.hpp"

int main(int argc, char *argv[]) {
    QApplication app{argc, argv};
    tracker test{};

    return app.exec();
}