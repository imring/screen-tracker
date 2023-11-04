#ifndef SCREEN_TRACKER_HPP
#define SCREEN_TRACKER_HPP

#include <mutex>

#include <QTimer>
#include <QImage>

class tracker : public QObject {
    Q_OBJECT

public:
    tracker(QObject *parent = nullptr);

private:
    void timer_tick();
    void compare_and_save(const QImage image);

    QTimer    *timer_;
    QImage     previous_image_;
    std::mutex mutex_;
};

#endif // SCREEN_TRACKER_HPP