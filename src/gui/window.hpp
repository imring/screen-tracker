#ifndef SCREEN_TRACKER_GUI_WINDOW_HPP
#define SCREEN_TRACKER_GUI_WINDOW_HPP

#include <QMainWindow>

#include "flowlayout.h"

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);

private:
    FlowLayout *flow_layout_ = nullptr;
};

#endif // SCREEN_TRACKER_GUI_WINDOW_HPP