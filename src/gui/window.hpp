#ifndef SCREEN_TRACKER_GUI_WINDOW_HPP
#define SCREEN_TRACKER_GUI_WINDOW_HPP

#include <QMainWindow>

#include "../tracker.hpp"
#include "flowlayout.h"

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void DisplayResult(const QImage &image, double similarity);

private:
    ScreenInterface *screen_interface_ = nullptr;
    Tracker         *tracker_          = nullptr;

    FlowLayout *flow_layout_ = nullptr;
};

#endif // SCREEN_TRACKER_GUI_WINDOW_HPP