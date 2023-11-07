#include "database.hpp"

#include <QVariant>
#include <QSqlQuery>

database::database(QObject *parent, QString path) : QSqlDatabase{"QSQLITE"} {
    setDatabaseName(path);
    if (!open()) {
        qDebug("Failed to open database.");
        return;
    }

    exec("CREATE TABLE IF NOT EXISTS screenshots (id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT, hash TEXT, similarity REAL)");
}

void database::add(const database::element &ele) {
    if (!isOpen()) {
        qDebug("The database is not open.");
        return;
    }
    QSqlQuery query{*this};
    query.prepare("INSERT INTO screenshots (path, hash, similarity) VALUES (:path, :hash, :similarity)");
    query.bindValue(":path", ele.path);
    query.bindValue(":hash", ele.hash);
    query.bindValue(":similarity", ele.similarity);
    if (!query.exec()) {
        qDebug("Failed to add the element.");
    }
}

QList<database::element> database::elements(bool reverse) const {
    if (!isOpen()) {
        qDebug("The database is not open.");
        return {};
    }
    QList<database::element> result;

    const QString query_str = reverse ? "SELECT path, hash, similarity FROM screenshots ORDER BY id DESC" : "SELECT path, hash, similarity FROM screenshots";
    QSqlQuery     query{*this};
    if (!query.exec(query_str)) {
        qDebug("Failed to get elements.");
        return {};
    }
    while (query.next()) {
        const QString path       = query.value(0).toString();
        const QString hash       = query.value(1).toString();
        const double  similarity = query.value(2).toDouble();
        result.push_back({path, hash, similarity});
    }
    return result;
}

database::element database::last_element() const {
    if (!isOpen()) {
        qDebug("The database is not open.");
        return {};
    }

    QSqlQuery query{*this};
    if (!query.exec("SELECT path, hash, similarity FROM screenshots ORDER BY id DESC LIMIT 1") || !query.first()) {
        qDebug("Failed to get the last element.");
        return {};
    }
    const QString path       = query.value(0).toString();
    const QString hash       = query.value(1).toString();
    const double  similarity = query.value(2).toDouble();
    return {path, hash, similarity};
}
