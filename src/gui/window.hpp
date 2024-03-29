#ifndef SCREEN_TRACKER_GUI_WINDOW_HPP
#define SCREEN_TRACKER_GUI_WINDOW_HPP

#include <QMainWindow>

#include "../tracker.hpp"
#include "../database.hpp"
#include "flowlayout.h"

class Window : public QMainWindow {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    Window(const Window &)            = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&)                 = delete;
    Window &operator=(Window &&)      = delete;
    ~Window();

private:
    void LoadLastImage();
    void LoadImage(std::size_t index);

    ScreenInterface   *screen_interface_ = nullptr;
    Tracker           *tracker_          = nullptr;
    DatabaseInterface *database_         = nullptr;

    FlowLayout *flow_layout_ = nullptr;

private slots:
    void DisplayResult(const QImage &image, double similarity, bool back = true);
    void SaveImage(const QImage &image, double similarity);
};

#endif // SCREEN_TRACKER_GUI_WINDOW_HPP