#ifndef SCREEN_TRACKER_DATABASE_QT_HPP
#define SCREEN_TRACKER_DATABASE_QT_HPP

#include <QSqlDatabase>

#include "../database.hpp"

class DatabaseQt : public DatabaseInterface {
public:
    explicit DatabaseQt(std::string_view name = QSqlDatabase::defaultConnection);
    DatabaseQt(const DatabaseQt &)            = delete;
    DatabaseQt &operator=(const DatabaseQt &) = delete;
    DatabaseQt(DatabaseQt &&)                 = delete;
    DatabaseQt &operator=(DatabaseQt &&)      = delete;
    ~DatabaseQt() override                    = default;

    void SetPath(std::string_view path) override;
    bool Open() override;
    bool Close() override;

    bool               AddImage(const QImage &image, double similarity) override;
    std::vector<Image> GetAllImages() override;
    std::size_t        GetCount() override;
    Image              At(std::size_t index) override;

private:
    QSqlDatabase database_;
};

#endif // SCREEN_TRACKER_DATABASE_QT_HPP