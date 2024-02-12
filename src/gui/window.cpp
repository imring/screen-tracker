#include <QScrollArea>

#include "../qt/screen_qt.hpp"
#include "window.hpp"
#include "imageview.hpp"

Window::Window(QWidget *parent) : QMainWindow{parent}, screen_interface_{new ScreenQt}, tracker_{new Tracker{screen_interface_}}, flow_layout_{new FlowLayout} {
    auto *widget = new QWidget{this};
    widget->setLayout(flow_layout_);

    auto *scroll_area = new QScrollArea{this};
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    setCentralWidget(scroll_area);

    connect(tracker_, &Tracker::OnTick, this, &Window::DisplayResult);
    tracker_->Start();
}

Window::~Window() {
    delete screen_interface_;
}

void Window::DisplayResult(const QImage &image, double similarity) {
    auto *image_view = new ImageView{image, similarity};
    flow_layout_->addWidget(image_view);
}
