#include <QScrollArea>

#include "window.hpp"
#include "imageview.hpp"

Window::Window(QWidget *parent) : QMainWindow{parent}, flow_layout_{new FlowLayout} {
    for (int i = 0; i < 10; i++) {
        auto *view2 = new ImageView{QImage{"assets/image2.png"}, 0.5};
        flow_layout_->addWidget(view2);
    }

    auto *widget = new QWidget{this};
    widget->setLayout(flow_layout_);

    auto *scroll_area = new QScrollArea{this};
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    setCentralWidget(scroll_area);
}
