#include "tracker.hpp"

#include <thread>

#include <QDebug>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QTextStream>
#include <QApplication>

using namespace std::chrono_literals;

QPixmap take_screenshot() {
    const QList<QScreen *> screens     = QGuiApplication::screens();
    int                    total_width = 0, total_height = 0;

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

    return combined;
}

// TODO: OpenCV?
double compare_images(const QImage &image1, const QImage &image2) {
    const double total     = qMax(image1.width() * image1.height(), image2.width() * image2.height()); // total pixels
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

tracker::tracker(QObject *parent) : QObject{parent}, timer_{new QTimer{this}} {
    timer_->setInterval(1s);

    connect(timer_, &QTimer::timeout, this, &tracker::timer_tick);
    timer_->start();
    timer_tick();
}

void tracker::timer_tick() {
    const QImage image = take_screenshot().toImage();
    std::thread{&tracker::compare_and_save, this, image}.detach();
}

void tracker::compare_and_save(const QImage image) {
    std::scoped_lock<std::mutex> lock{mutex_};
    if (previous_image_.isNull()) {
        qInfo("100%");
    } else {
        const double result = compare_images(previous_image_, image);
        qInfo() << Qt::fixed << qSetRealNumberPrecision(2) << (result * 100) << "%";
    }
    previous_image_ = image;
}