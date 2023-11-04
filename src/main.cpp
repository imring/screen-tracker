#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QApplication>
#include <QImageReader>

#include <iostream>

bool take_screenshot(std::string_view path) {
    QList<QScreen *> screens     = QGuiApplication::screens();
    int              total_width = 0, total_height = 0;

    for (QScreen *screen: screens) {
        const auto geometry = screen->geometry();
        total_width         = qMax(total_width, geometry.right());
        total_height        = qMax(total_height, geometry.bottom());
    }

    QPixmap  combined{total_width, total_height};
    QPainter painter{&combined};

    for (QScreen *screen: screens) {
        const auto geometry   = screen->geometry();
        QPixmap    screenshot = screen->grabWindow(0);
        painter.drawPixmap(geometry.left(), geometry.top(), screenshot);
    }

    return combined.save(QString::fromUtf8(path.data(), path.size()));
}

// TODO: OpenCV?
double compare_images(const QImage &image1, const QImage &image2) {
    const double total     = qMax(image1.width() * image1.height(), image2.width() * image2.height());  // total pixels
    double       different = 0;
    if (image1.size() != image2.size()) {
        // if the sizes aren't the same, then add the missing pixels
        different += qAbs(image1.width() * image1.height() - image2.width() * image2.height());
    }

    for (int x = 0; x < qMin(image1.width(), image2.width()); ++x) {
        for (int y = 0; y < qMin(image1.height(), image2.height()); ++y) {
            if (image1.pixelColor(x, y) != image2.pixelColor(x, y)) {
                different++;
            }
        }
    }
    return 1.0 - different / total;
}

int main(int argc, char *argv[]) {
    QApplication app{argc, argv};
    take_screenshot("screenshot.png");

    std::cout << compare_images(QImageReader{"screenshot.png"}.read(), QImageReader{"screenshot_old.png"}.read()) << std::endl;

    return 0;
}