#ifndef SCREEN_TRACKER_DATABASE_QT_HPP
#define SCREEN_TRACKER_DATABASE_QT_HPP

#include <QSqlDatabase>

#include "../database.hpp"

class DatabaseQt : public DatabaseInterface {
public:
    DatabaseQt(std::string_view name = QSqlDatabase::defaultConnection);
    ~DatabaseQt() override = default;

    void SetPath(std::string_view path) override;
    bool Open() override;
    bool Close() override;

    bool               AddImage(const QImage &image, double similarity) override;
    std::vector<Image> GetAllImages() override;

private:
    QSqlDatabase database;
};

#endif // SCREEN_TRACKER_DATABASE_QT_HPP