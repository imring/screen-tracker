#include <QCryptographicHash>
#include <QImage>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

#include "database_qt.hpp"

DatabaseQt::DatabaseQt(std::string_view name) : database_{QSqlDatabase::addDatabase("QSQLITE", name.data())} {}

void DatabaseQt::SetPath(std::string_view path) {
    database_.setDatabaseName(path.data());
}

bool DatabaseQt::Open() {
    if (!database_.open()) {
        return false;
    }
    database_.exec("CREATE TABLE IF NOT EXISTS screens (id INTEGER PRIMARY KEY, image BLOB, hash TEXT, similarity REAL);");
    return true;
}

bool DatabaseQt::Close() {
    return true;
}

bool DatabaseQt::AddImage(const QImage &image, double similarity) {
    QSqlQuery query{database_};
    query.prepare("INSERT INTO screens (image, hash, similarity) VALUES (?, ?, ?);");

    const QByteArray array = Image::ImageToArray(image);
    query.bindValue(0, QVariant{array}, QSql::Binary);
    query.bindValue(1, Image::HashArray(array));
    query.bindValue(2, similarity);
    return query.exec();
}

std::vector<Image> DatabaseQt::GetAllImages() {
    QSqlQuery query{database_};
    query.prepare("SELECT image, hash, similarity FROM screens;");
    if (!query.exec()) {
        return {};
    }

    std::vector<Image> images;
    while (query.next()) {
        const QByteArray array      = query.value(0).toByteArray();
        const QString    hash       = query.value(1).toString();
        const double     similarity = query.value(2).toDouble();

        images.emplace_back(array, hash, similarity);
    }
    return images;
}

std::size_t DatabaseQt::GetCount() {
    QSqlQuery query{database_};
    query.prepare("SELECT COUNT(*) FROM screens;");
    if (!query.exec() || !query.next()) {
        return 0;
    }
    return query.value(0).toInt();
}

Image DatabaseQt::At(std::size_t index) {
    QSqlQuery query{database_};
    query.prepare("SELECT image, hash, similarity FROM screens WHERE id = ?;");
    query.bindValue(0, index);
    if (!query.exec() || !query.next()) {
        return {};
    }

    const QByteArray array      = query.value(0).toByteArray();
    const QString    hash       = query.value(1).toString();
    const double     similarity = query.value(2).toDouble();
    return Image{array, hash, similarity};
}