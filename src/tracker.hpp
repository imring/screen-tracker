#ifndef SCREEN_TRACKER_HPP
#define SCREEN_TRACKER_HPP

#include <mutex>

#include <QTimer>
#include <QImage>
#include <QSqlDatabase>

class database : public QSqlDatabase {
public:
    database(QObject *parent = nullptr, QString path = "database.db");

    struct element {
        QString path;
        QString hash;
        double  similarity = 0;
        bool    checksum   = false;
    };

    void                 add(const QString &path, const QString &hash, double similarity);
    std::vector<element> elements() const;
    element              last_element() const;
};

class tracker : public QObject {
    Q_OBJECT

public:
    tracker(QObject *parent = nullptr);

private:
    void timer_tick();
    void compare_and_save(const QImage image);

    QTimer     timer_;
    std::mutex mutex_;
    database   db_;
};

#endif // SCREEN_TRACKER_HPP