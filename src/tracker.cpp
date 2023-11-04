#include "tracker.hpp"

#include <thread>
#include <filesystem>

#include <QDebug>
#include <QBuffer>
#include <QScreen>
#include <QPixmap>
#include <QPainter>
#include <QFileInfo>
#include <QSqlQuery>
#include <QTextStream>
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

// TODO: OpenCV?
double compare_images(const QImage &image1, const QImage &image2) {
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
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(array);
    QByteArray result = hash.result();
    return result.toHex();
}

QString sha_hash(const QImage &image) {
    if (image.isNull()) {
        return {};
    }
    return sha_hash(image_to_array(image));
}

// database

database::database(QObject *parent, QString path) : QSqlDatabase{"QSQLITE"} {
    setDatabaseName(path);
    if (!open()) {
        return;
    }

    exec("CREATE TABLE IF NOT EXISTS screenshots (id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, hash TEXT, similarity REAL)");
}

void database::add(const QString &path, const QString &hash, double similarity) {
    if (!isOpen()) {
        return;
    }
    QSqlQuery query{*this};
    query.prepare("INSERT INTO screenshots (path, hash, similarity) VALUES (:path, :hash, :similarity)");
    query.bindValue(":path", path);
    query.bindValue(":hash", hash);
    query.bindValue(":similarity", similarity);
    query.exec();
}

std::vector<database::element> database::elements() const {
    if (!isOpen()) {
        return {};
    }
    std::vector<database::element> result;

    QSqlQuery query{*this};
    query.exec("SELECT path, hash, similarity FROM screenshots");
    while (query.next()) {
        const QString path       = query.value(0).toString();
        const QString hash       = query.value(1).toString();
        const double  similarity = query.value(2).toDouble();
        const bool    checksum   = sha_hash(QImage{path, image_format}) == hash;
        result.emplace_back(path, hash, similarity, checksum);
    }
    return result;
}

database::element database::last_element() const {
    if (!isOpen()) {
        return {};
    }

    QSqlQuery query{*this};
    if (!query.exec("SELECT path, hash, similarity FROM screenshots ORDER BY id DESC LIMIT 1") || !query.first()) {
        return {};
    }
    const QString path       = query.value(0).toString();
    const QString hash       = query.value(1).toString();
    const double  similarity = query.value(2).toDouble();
    const bool    checksum   = sha_hash(QImage{path, image_format}) == hash;
    return {path, hash, similarity, checksum};
}

// tracker

tracker::tracker(QObject *parent) : QObject{parent}, timer_{this}, db_{this} {
    timer_.setInterval(1min);

    connect(&timer_, &QTimer::timeout, this, &tracker::timer_tick);
    timer_.start();
    timer_tick();
}

void tracker::timer_tick() {
    const QImage image = take_screenshot().toImage();
    std::thread{&tracker::compare_and_save, this, image}.detach();
}

void tracker::compare_and_save(const QImage image) {
    std::scoped_lock<std::mutex> lock{mutex_};

    const database::element prev = db_.last_element();
    QImage                  previous_image;
    if (!prev.path.isEmpty() && QFileInfo::exists(prev.path)) {
        previous_image.load(prev.path, image_format);
    }

    double result = 1;
    if (!previous_image.isNull()) {
        result = compare_images(previous_image, image);
    }

    const auto    now      = std::chrono::system_clock::now();
    const auto    current  = std::filesystem::current_path() / "images" / std::format("image-{:%Y%m%d-%H%M%S}.png", now);
    const QString qcurrent = QString::fromStdString(current.string());
    image.save(qcurrent, image_format);
    db_.add(qcurrent, sha_hash(image), result);
    qInfo() << result << ' ' << qcurrent;
}