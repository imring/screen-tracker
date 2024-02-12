#include <QLabel>
#include <QVBoxLayout>

#include "imageview.hpp"

const QSize      image_size{200, 100};
constexpr double to_percentage = 100.0;

ImageView::ImageView(const QImage &image, double similarity, QWidget *parent) : QWidget{parent} {
    auto *layout      = new QVBoxLayout;
    auto *image_label = new QLabel;

    const bool image_valid = !image.isNull();
    if (image_valid) {
        const QImage image_scaled = image.scaled(image_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        image_label->setPixmap(QPixmap::fromImage(image_scaled));
    } else {
        image_label->setText("Invalid image");
    }
    image_label->setFixedSize(image_size);
    image_label->setAlignment(Qt::AlignCenter);

    auto *similarity_label = new QLabel;
    if (image_valid) {
        similarity_label->setText(std::format("Similarity: {:.02f}%", similarity * to_percentage).c_str());
        similarity_label->setAlignment(Qt::AlignHCenter);
    }

    layout->addWidget(image_label);
    layout->addWidget(similarity_label);
    setLayout(layout);
    adjustSize();
}