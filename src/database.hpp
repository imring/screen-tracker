#ifndef SCREEN_TRACKER_DATABASE_HPP
#define SCREEN_TRACKER_DATABASE_HPP

#include <QImage>

struct Image {
    Image() = default;
    Image(const QByteArray &array, const QString &hash, double similarity);

    QImage image;
    double similarity = 0.0;

    bool IsValid() const {
        return !image.isNull();
    }

    static QByteArray ImageToArray(const QImage &image);
    static QString    HashArray(const QByteArray &array);

    bool operator==(const Image &) const = default;
};

class DatabaseInterface {
public:
    virtual ~DatabaseInterface() = default;

    virtual void SetPath(std::string_view path) = 0;
    virtual bool Open()                         = 0;
    virtual bool Close()                        = 0;

    virtual bool               AddImage(const QImage &image, double similarity) = 0;
    virtual std::vector<Image> GetAllImages()                                   = 0;
    virtual std::size_t        GetCount()                                       = 0;
    virtual Image              At(std::size_t index)                            = 0;
};

#endif // SCREEN_TRACKER_DATABASE_HPP