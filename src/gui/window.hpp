#ifndef SCREEN_TRACKER_GUI_WINDOW_HPP
#define SCREEN_TRACKER_GUI_WINDOW_HPP

#include <QMainWindow>

#include "../tracker.hpp"
#include "../database.hpp"
#include "flowlayout.h"

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

private:
    ScreenInterface *screen_interface_ = nullptr;
    Tracker         *tracker_          = nullptr;

    DatabaseInterface *database_ = nullptr;

    FlowLayout *flow_layout_ = nullptr;

private slots:
    void DisplayResult(const QImage &image, double similarity);
    void SaveImage(const QImage &image, double similarity);
};

#endif // SCREEN_TRACKER_GUI_WINDOW_HPP