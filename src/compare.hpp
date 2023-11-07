#ifndef SCREEN_TRACKER_COMPARE_HPP
#define SCREEN_TRACKER_COMPARE_HPP

#include <QImage>

class compare_worker : public QObject {
    Q_OBJECT
public:
    compare_worker(const QImage &previous, const QImage &current, QObject *parent = nullptr);

    void process();

signals:
    void on_compare(const QImage &image, double similarity);

private:
    QImage previous_, current_;
};

#endif // SCREEN_TRACKER_COMPARE_HPP