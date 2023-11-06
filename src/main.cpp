#include <QApplication>

#include "window.hpp"

int main(int argc, char *argv[]) {
    QApplication app{argc, argv};
    window       win;

    return app.exec();
}