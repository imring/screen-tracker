#include "tracker.hpp"
#include "database.hpp"

#include <filesystem>

#include <QThread>
#include <QBuffer>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include <QApplication>
#include <QImageWriter>
#include <QCryptographicHash>

using namespace std::chrono_literals;

constexpr const char *image_format = "PNG";

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

QByteArray image_to_array(const QImage &image) {
    QByteArray array;
    QBuffer    buffer{&array};
    buffer.open(QIODevice::WriteOnly);

    QImageWriter writer{&buffer, image_format};
    writer.write(image);
    buffer.close();
    return array;
}

QString sha_hash(const QByteArray &array) {
    QCryptographicHash hash{QCryptographicHash::Sha256};
    hash.addData(array);
    QByteArray result = hash.result();
    return result.toHex();
}

// tracker

tracker::tracker(QObject *parent) : QObject{parent}, timer_{this} {
    timer_.setInterval(1min);

    connect(&timer_, &QTimer::timeout, this, &tracker::timer_tick);
    timer_.start();
    timer_tick();
}

void tracker::timer_tick() {
    const QImage image = take_screenshot().toImage();
    database     db{this};

    const database::element prev = db.last_element();
    QImage                  previous_image;
    if (!prev.path.isEmpty() && QFileInfo::exists(prev.path)) {
        previous_image.load(prev.path, image_format);
    }

    if (!previous_image.isNull()) {
        compare_worker *worker = new compare_worker{previous_image, image};
        QThread        *thread = new QThread{this};
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &compare_worker::process);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(worker, &compare_worker::on_compare, this, &tracker::save);

        thread->start();
    } else { // just save
        save(image, 1.0);
    }
}

void tracker::save(const QImage &image, double similarity) {
    const auto    now      = std::chrono::system_clock::now();
    const auto    current  = std::filesystem::current_path() / "images" / std::format("image-{:%Y%m%d-%H%M%S}.png", now);
    const QString qcurrent = QString::fromStdString(current.string());

    // save image
    // array is used for saving and hashing
    const QByteArray array = image_to_array(image);
    QFile            file{qcurrent};
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    file.write(array);
    file.close();

    database db{this};
    db.add(qcurrent, sha_hash(array), similarity);
}