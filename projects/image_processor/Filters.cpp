
#include "Filters.h"
#include <iostream>
#include <math.h>
template <typename T>
Color FilterWithMatrix<T>::ApplyMatrix(size_t height, size_t width, Image& image) {
    Color pixel_out(0, 0, 0);
    size_t x_coord = 0;
    size_t y_coord = 0;
    for (int64_t i = height - matrix_.size() / 2; i < static_cast<int64_t>(height + matrix_.size() / 2 + 1); ++i) {
        for (int64_t j = width - matrix_[0].size() / 2; j < static_cast<int64_t>(width + matrix_.size() / 2 + 1); ++j) {
            int64_t negative_h_distance = std::min(static_cast<int64_t>(0), i);
            int64_t positive_h_distance =
                std::min(static_cast<int64_t>(0), static_cast<int64_t>(image.GetHeight()) - i - 1);
            int64_t negative_w_distance = std::min(static_cast<int64_t>(0), j);
            int64_t positive_w_distance =
                std::min(static_cast<int64_t>(0), static_cast<int64_t>(image.GetWidth()) - j - 1);
            pixel_out += image.GetPixel(i - negative_h_distance + positive_h_distance,
                                        j - negative_w_distance + positive_w_distance) *
                         matrix_[y_coord][x_coord];
            ++x_coord;
        }
        x_coord = 0;
        ++y_coord;
    }
    if (pixel_out.red > 1) {
        pixel_out.red = 1;
    } else if (pixel_out.red < 0) {
        pixel_out.red = 0;
    }
    if (pixel_out.green > 1) {
        pixel_out.green = 1;
    } else if (pixel_out.green < 0) {
        pixel_out.green = 0;
    }
    if (pixel_out.blue > 1) {
        pixel_out.blue = 1;
    } else if (pixel_out.blue < 0) {
        pixel_out.blue = 0;
    }

    return pixel_out;
}

template <typename T>
void FilterWithMatrix<T>::Apply(Image& image) {
    Image image_new(image.GetHeight(), image.GetWidth());
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            image_new.SetPixel(i, j, ApplyMatrix(i, j, image));
        }
    }
    image = image_new;
}

template <typename T>
FilterWithMatrix<T>::FilterWithMatrix(FilterMatrix<T>& matrix) {
    matrix_ = matrix;
}

SharpeningFilter::SharpeningFilter(FilterMatrix<int> matrix) : FilterWithMatrix<int>(matrix) {}

void Grayscale::Apply(Image& image) {
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            Color pixel = image.GetPixel(i, j);
            double new_col = 0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue;
            Color new_pixel(new_col, new_col, new_col);
            image.SetPixel(i, j, new_pixel);
        }
    }
}

Crop::Crop(size_t height, size_t width) : width_(width), height_(height) {
}

void Crop::Apply(Image& image) {
    ImageMatrix cropped;
    size_t iteration = 0;
    for (size_t i = image.GetHeight() - std::min(height_, image.GetHeight()); i < image.GetHeight(); ++i) {
        cropped.push_back({});
        for (size_t j = 0; j < std::min(width_, image.GetWidth()); ++j) {
            cropped[iteration].push_back(image.GetPixel(i, j));
        }
        ++iteration;
    }
    image.SetImageMatrix(cropped);
}

EdgeThreshold::EdgeThreshold(float thresh, FilterMatrix<int> matrix) : FilterWithMatrix<int>(matrix) {
    thresh_ = thresh;
}

void EdgeThreshold::Apply(Image& image) {
    Grayscale grey;
    grey.Apply(image);
    FilterWithMatrix::Apply(image);
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            if (image.GetPixel(i, j).red < thresh_) {
                image.SetPixel(i, j, Color(0, 0, 0));
            } else {
                image.SetPixel(i, j, Color(1, 1, 1));
            }
        }
    }
}

void Negative::Apply(Image& image) {
    for (size_t i = 0; i < image.GetHeight(); ++i) {
        for (size_t j = 0; j < image.GetWidth(); ++j) {
            image.SetPixel(i, j, Color(1, 1, 1) + image.GetPixel(i, j) * -1);
        }
    }
}

FilterMatrix<double> GaussianBlur::MakeGauss() {
    const double pi = std::atan(1) * 4;
    FilterMatrix<double> gauss_filter;
    int mat_size = std::ceil(6 * sigma_) + static_cast<int>(std::ceil(6 * sigma_) + 1) % 2;
    double summary = 0;
    for (int i = 0; i < mat_size; ++i) {
        gauss_filter.push_back({});
        for (int j = 0; j < mat_size; ++j) {
            double exp_part =
                std::exp(-1 * (std::pow(std::abs(mat_size / 2 - i), 2) + std::pow(std::abs(mat_size / 2 - j), 2)) /
                         (2 * std::pow(sigma_, 2)));
            double gauss_ij_element = (1 / std::sqrt(pi * std::pow(sigma_, 2))) * exp_part;
            summary += gauss_ij_element;
            gauss_filter[i].push_back(gauss_ij_element);
        }
    }
    for (int i = 0; i < mat_size; ++i) {
        for (int j = 0; j < mat_size; ++j) {
            gauss_filter[i][j] = gauss_filter[i][j] * (1 / summary);
        }
    }
    return gauss_filter;
}

GaussianBlur::GaussianBlur(float sigma, FilterMatrix<double> matrix) : FilterWithMatrix<double>(matrix) {
    sigma_ = sigma;
    this->matrix_ = MakeGauss();
}

HorizontalIntegralBlur::HorizontalIntegralBlur(FilterMatrix<double> matrix) : FilterWithMatrix<double>(matrix) {
}

void HorizontalIntegralBlur::Apply(Image& image) {
    for (size_t i = 0; i < image.GetWidth(); ++i) {
        for (size_t j = 0; j < image.GetHeight(); ++j) {
            image.SetPixel(j, i, ApplyMatrix(j, i, image));
        }
    }
}
