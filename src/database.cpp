#include <QBuffer>
#include <QCryptographicHash>

#include "database.hpp"

Image::Image(const QByteArray &array, const QString &hash, double similarity) {
    if (HashArray(array) != hash) {
        return;
    }

    this->image      = QImage::fromData(array);
    this->similarity = similarity;
}

QByteArray Image::ImageToArray(const QImage &image) {
    QByteArray array;
    QBuffer    buffer{&array};
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return array;
}

QString Image::HashArray(const QByteArray &array) {
    QCryptographicHash hash{QCryptographicHash::Sha256};
    hash.addData(array);
    QByteArray result = hash.result();
    return result.toHex();
}