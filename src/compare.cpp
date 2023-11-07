#include "compare.hpp"

#include "cvmatandqimage.h"

double compare(const QImage &image1, const QImage &image2) {
    if (image1.size() != image2.size()) {
        return 0;
    }

    const cv::Mat mat1 = QtOcv::image2Mat(image1);
    const cv::Mat mat2 = QtOcv::image2Mat(image2);

    cv::Mat diff;
    cv::absdiff(mat1, mat2, diff);
    const cv::Scalar mean   = cv::mean(diff);
    const double     result = (mean[0] + mean[1] + mean[2]) / 3.0 / 255.0;

    return 1.0 - result;
}

compare_worker::compare_worker(const QImage &previous, const QImage &current, QObject *parent) : QObject{parent}, previous_{previous}, current_{current} {}

void compare_worker::process() {
    const double result = compare(previous_, current_);
    emit         on_compare(current_, result);
}