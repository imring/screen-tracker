#ifndef SCREEN_TRACKER_DATABASE_HPP
#define SCREEN_TRACKER_DATABASE_HPP

#include <QObject>
#include <QSqlDatabase>

class database : public QSqlDatabase {
public:
    database(QObject *parent = nullptr, QString path = "database.db");

    struct element {
        QString path;
        QString hash;
        double  similarity = 0;
    };

    void           add(const QString &path, const QString &hash, double similarity);
    QList<element> elements() const;
    element        last_element() const;
};

#endif // SCREEN_TRACKER_DATABASE_HPP