#ifndef SCREEN_TRACKER_SCREEN_HPP
#define SCREEN_TRACKER_SCREEN_HPP

#include <QPixmap>

class QImage;

class Screen {
public:
    static QPixmap Capture();
    static double Compare(const QImage &left, const QImage &right);
};

#endif // SCREEN_TRACKER_SCREEN_HPP