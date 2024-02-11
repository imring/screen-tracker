#ifndef SCREEN_TRACKER_SCREEN_QT_HPP
#define SCREEN_TRACKER_SCREEN_QT_HPP

#include "../screen.hpp"

class ScreenQt : public ScreenInterface {
public:
    QPixmap Capture() override;
    double  Compare(const QImage &left, const QImage &right) override;
};

#endif // SCREEN_TRACKER_SCREEN_QT_HPP