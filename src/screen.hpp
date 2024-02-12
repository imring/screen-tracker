#ifndef SCREEN_TRACKER_SCREEN_HPP
#define SCREEN_TRACKER_SCREEN_HPP

#include <QPixmap>

class ScreenInterface {
public:
    virtual ~ScreenInterface()                                       = default;
    virtual QPixmap Capture()                                        = 0;
    virtual double  Compare(const QImage &left, const QImage &right) = 0;
};

#endif // SCREEN_TRACKER_SCREEN_HPP