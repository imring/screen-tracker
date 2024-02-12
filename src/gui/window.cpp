#include <QScrollArea>

#include "../qt/screen_qt.hpp"
#include "../qt/database_qt.hpp"

#include "window.hpp"
#include "imageview.hpp"

Window::Window(QWidget *parent)
    : QMainWindow{parent},
      screen_interface_{new ScreenQt},
      tracker_{new Tracker{screen_interface_}},
      database_{new DatabaseQt},
      flow_layout_{new FlowLayout} {
    auto *widget = new QWidget{this};
    widget->setLayout(flow_layout_);

    auto *scroll_area = new QScrollArea{this};
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(widget);

    setCentralWidget(scroll_area);

    // database
    database_->SetPath("./database.db");
    database_->Open();
    const std::vector<Image> images = database_->GetAllImages();
    for (const auto &image: images) {
        DisplayResult(image.image, image.similarity);
    }
    if (images.size() != 0) {
        tracker_->SetPreviousImage(images.back().image);
    }

    // tracker
    connect(tracker_, &Tracker::OnTick, this, &Window::DisplayResult);
    connect(tracker_, &Tracker::OnTick, this, &Window::SaveImage);
    tracker_->Start();
}

Window::~Window() {
    database_->Close();
    tracker_->Stop();

    delete screen_interface_;
    delete database_;
}

void Window::DisplayResult(const QImage &image, double similarity) {
    auto *image_view = new ImageView{image, similarity};
    flow_layout_->addWidget(image_view);
}

void Window::SaveImage(const QImage &image, double similarity) {
    database_->AddImage(image, similarity);
}