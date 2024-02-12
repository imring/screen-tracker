#include <QGuiApplication>
#include <QPainter>
#include <QScreen>

#include "screen_qt.hpp"

constexpr double max_color    = 255.0;
constexpr double color_number = 3.0;

double compare_color(int left, int right) {
    return 1 - static_cast<double>(std::abs(left - right)) / max_color;
}

QPixmap ScreenQt::Capture() {
    const QList<QScreen *> screens = QGuiApplication::screens();
    if (screens.empty()) {
        return {};
    }

    // calc size of all screens
    int total_width  = 0;
    int total_height = 0;
    for (QScreen *screen: screens) {
        const QRect geometry = screen->geometry();
        total_width          = qMax(total_width, geometry.right());
        total_height         = qMax(total_height, geometry.bottom());
    }

    // render to a pixmap
    QPixmap  combined{total_width, total_height};
    QPainter painter{&combined};
    for (QScreen *screen: screens) {
        const QRect   geometry   = screen->geometry();
        const QPixmap screenshot = screen->grabWindow(0);
        painter.drawPixmap(geometry.left(), geometry.top(), screenshot);
    }

    return combined;
}

double ScreenQt::Compare(const QImage &left, const QImage &right) {
    double value = 0;
    for (int i = 0; i < left.width(); i++) {
        for (int j = 0; j < left.height(); j++) {
            const QColor pixel1{left.pixel(i, j)};
            const QColor pixel2{right.pixel(i, j)};

            // calculate the average difference in color values
            const double pixel_diff_red   = compare_color(pixel1.red(), pixel2.red());
            const double pixel_diff_green = compare_color(pixel1.green(), pixel2.green());
            const double pixel_diff_blue  = compare_color(pixel1.blue(), pixel2.blue());
            value += (pixel_diff_red + pixel_diff_green + pixel_diff_blue) / color_number;
        }
    }
    return value / (left.width() * left.height());
}