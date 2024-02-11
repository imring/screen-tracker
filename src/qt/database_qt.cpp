#include <QImage>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QCryptographicHash>

#include "database_qt.hpp"

DatabaseQt::DatabaseQt(std::string_view name) : database{QSqlDatabase::addDatabase("QSQLITE", name.data())} {}

void DatabaseQt::SetPath(std::string_view path) {
    database.setDatabaseName(path.data());
}

bool DatabaseQt::Open() {
    if (!database.open()) {
        return false;
    }
    database.exec("CREATE TABLE IF NOT EXISTS screens (id INTEGER PRIMARY KEY, image BLOB, hash TEXT, similarity REAL);");
    return true;
}

bool DatabaseQt::Close() {
    database.close();
    return true;
}

bool DatabaseQt::AddImage(const QImage &image, double similarity) {
    QSqlQuery query{database};
    query.prepare("INSERT INTO screens (image, hash, similarity) VALUES (?, ?, ?);");

    const QByteArray array = Image::ImageToArray(image);
    query.bindValue(0, QVariant{array}, QSql::Binary);
    query.bindValue(1, Image::HashArray(array));
    query.bindValue(2, similarity);
    return query.exec();
}

std::vector<Image> DatabaseQt::GetAllImages() {
    QSqlDatabase database = QSqlDatabase::database();
    QSqlQuery    query{database};
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