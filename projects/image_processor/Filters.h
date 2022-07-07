#pragma once
#include "Image.h"
#include <functional>
template <typename T>
using FilterMatrix = std::vector<std::vector<T>>;
class Filter {
public:
    virtual void Apply(Image& image) = 0;
};
template <typename T>
class FilterWithMatrix : public Filter {
public:
    virtual Color ApplyMatrix(size_t height, size_t width, Image& image);
    virtual void Apply(Image& image);
    FilterWithMatrix(FilterMatrix<T>& matrix);

protected:
    FilterMatrix<T> matrix_ = {};
};

class SharpeningFilter : public FilterWithMatrix<int> {
public:
    SharpeningFilter(FilterMatrix<int> matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}});
};

class Grayscale : public Filter {
public:
    void Apply(Image& image);
};

class Crop : public Filter {
public:
    void Apply(Image& image);
    Crop(size_t width, size_t height);

private:
    size_t width_ = 0;
    size_t height_ = 0;
};

class EdgeThreshold : public FilterWithMatrix<int> {
public:
    void Apply(Image& image) override;
    EdgeThreshold(float thresh, FilterMatrix<int> matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}});

private:
    float thresh_ = 0;
};

class Negative : public Filter {
public:
    void Apply(Image& image) override;
};

class GaussianBlur : public FilterWithMatrix<double> {
public:
    FilterMatrix<double> MakeGauss();
    GaussianBlur(float sigma, FilterMatrix<double> matrix = {});

private:
    double sigma_;
};

class HorizontalIntegralBlur : public FilterWithMatrix<double> {
public:
    void Apply(Image& image) override;
    HorizontalIntegralBlur(FilterMatrix<double> matrix = {{0.4, 0.2, 0.2, 0.1, 0.1, 0, 0, 0, 0}

                           });
};