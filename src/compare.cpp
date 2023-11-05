#include "compare.hpp"

// TODO: OpenCV?
double compare(const QImage &image1, const QImage &image2) {
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

compare_worker::compare_worker(QImage previous, QImage current, QObject *parent) : QObject{parent}, previous_{previous}, current_{current} {}

void compare_worker::process() {
    const double result = compare(previous_, current_);
    emit on_compare(current_, result);
}