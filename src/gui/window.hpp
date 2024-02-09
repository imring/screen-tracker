#ifndef SCREEN_TRACKER_GUI_WINDOW_HPP
#define SCREEN_TRACKER_GUI_WINDOW_HPP

#include <QMainWindow>

class Window : public QMainWindow {
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
};

#endif // SCREEN_TRACKER_GUI_WINDOW_HPP