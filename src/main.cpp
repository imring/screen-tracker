#include <QApplication>

#include "gui/window.hpp"

int main(int argc, char *argv[]) {
    const QApplication app{argc, argv};
    Window window;

    window.show();
    return app.exec();
}