#include <QLabel>
#include <QVBoxLayout>

#include "imageview.hpp"

const QSize image_size{200, 100};

ImageView::ImageView(const QImage &image, double similarity, QWidget *parent) : QWidget{parent} {
    const QImage image_scaled = image.scaled(image_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    auto *layout      = new QVBoxLayout;
    auto *image_label = new QLabel;
    image_label->setPixmap(QPixmap::fromImage(image_scaled));
    image_label->setFixedSize(image_size);
    image_label->setAlignment(Qt::AlignHCenter);

    auto *similarity_label = new QLabel;
    similarity_label->setText(std::format("Similarity: {:.02f}%", similarity * 100).c_str());
    similarity_label->setAlignment(Qt::AlignHCenter);

    layout->addWidget(image_label);
    layout->addWidget(similarity_label);
    setLayout(layout);
    adjustSize();
}