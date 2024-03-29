#ifndef SCREEN_TRACKER_TRACKER_HPP
#define SCREEN_TRACKER_TRACKER_HPP

#include <QImage>
#include <QMutex>
#include <QTimer>

#include "screen.hpp"

class Tracker : public QObject {
    Q_OBJECT
public:
    explicit Tracker(QObject *parent = nullptr);
    explicit Tracker(ScreenInterface *screen, QObject *parent = nullptr);
    Tracker(const QImage &prev_image, ScreenInterface *screen, QObject *parent = nullptr);

    void SetInterval(int msec);
    void SetPreviousImage(const QImage &image);

    void Start();
    void Stop();

#if __has_include(<chrono>)
    void SetInterval(std::chrono::milliseconds value) {
        SetInterval(static_cast<int>(value.count()));
    }
#endif

signals:
    void OnTick(const QImage &image, double similarity);

private:
    void Tick();

    QMutex           mutex_;
    QTimer           timer_;
    QImage           prev_image_;
    ScreenInterface *screen_ = nullptr;
};

#endif // SCREEN_TRACKER_TRACKER_HPP