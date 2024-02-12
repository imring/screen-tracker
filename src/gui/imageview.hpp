#ifndef SCREEN_TRACKER_GUI_IMAGEVIEW_HPP
#define SCREEN_TRACKER_GUI_IMAGEVIEW_HPP

#include <QWidget>

class ImageView : public QWidget {
public:
    ImageView(const QImage &image, double similarity, QWidget *parent = nullptr);
};

#endif // SCREEN_TRACKER_GUI_IMAGEVIEW_HPP