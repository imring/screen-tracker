#ifndef SCREEN_TRACKER_HPP
#define SCREEN_TRACKER_HPP

#include <QTimer>
#include <QImage>

#include "compare.hpp"

class tracker : public QObject {
    Q_OBJECT

public:
    tracker(QObject *parent = nullptr);

private slots:
    void save(const QImage &image, double similarity);

private:
    void timer_tick();

    QTimer timer_;
};

#endif // SCREEN_TRACKER_HPP