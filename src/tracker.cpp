#include <QThread>

#include "tracker.hpp"

class Worker : public QObject {
    Q_OBJECT

public slots:
    void CaptureAndCompare(const QImage &prev_image, ScreenInterface *screen) {
        const QImage current    = screen->Capture().toImage();
        double       similarity = 1.0;
        if (!prev_image.isNull()) {
            similarity = screen->Compare(prev_image, current);
        }
        emit ResultReady(current, similarity);
    }

signals:
    void ResultReady(const QImage &image, double similarity);
};

Tracker::Tracker(QObject *parent) : Tracker{nullptr, parent} {}
Tracker::Tracker(ScreenInterface *screen, QObject *parent) : Tracker{QImage{}, screen, parent} {}

Tracker::Tracker(const QImage &prev_image, ScreenInterface *screen, QObject *parent) : QObject{parent}, prev_image_{prev_image}, screen_{screen} {
    SetInterval(5000);
    connect(&timer_, &QTimer::timeout, this, &Tracker::Tick);
}

void Tracker::SetInterval(int msec) {
    timer_.setInterval(msec);
}

void Tracker::SetPreviousImage(const QImage &image) {
    prev_image_ = image;
}

void Tracker::Start() {
    timer_.start();
}

void Tracker::Stop() {
    timer_.stop();
}

void Tracker::Tick() {
    Worker  *worker        = new Worker;
    QThread *worker_thread = new QThread;
    worker->moveToThread(worker_thread);

    connect(worker_thread, &QThread::started, [this, worker]() {
        mutex_.lock();
        worker->CaptureAndCompare(prev_image_, screen_);
        mutex_.unlock();
    });

    connect(worker_thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker_thread, &QThread::finished, worker_thread, &QObject::deleteLater);

    connect(worker, &Worker::ResultReady, [this](const QImage &image, double /*similarity*/) {
        prev_image_ = image;
    });
    connect(worker, &Worker::ResultReady, this, &Tracker::OnTick);
    connect(worker, &Worker::ResultReady, worker_thread, &QThread::quit);

    worker_thread->start();
}

#include "tracker.moc"