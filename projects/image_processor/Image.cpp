#include <vector>
#include <string>
#include <fstream>
#include "Image.h"
#include <iostream>
#include <memory>
void BMP::Write(std::string path_to_file, Image& img) {
    std::ofstream file;
    file.open(path_to_file, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("File opening error");
    }
    unsigned char padding[3] = {0, 0, 0};
    const int size_of_padding = (4 - (img.GetWidth() * 3) % 4) % 4;
    const int file_size =
        INFOHEADERSIZE + DIBHEADERSIZE + size_of_padding * img.GetHeight() + img.GetHeight() * img.GetWidth() * 3;
    InformationHeader inf_header(file_size);
    DIB_header dib_header(img.GetWidth(), img.GetHeight());
    file.write(reinterpret_cast<char*>(inf_header.header), INFOHEADERSIZE);
    file.write(reinterpret_cast<char*>(dib_header.header), DIBHEADERSIZE);
    for (size_t h = 0; h < img.GetHeight(); ++h) {

        for (size_t w = 0; w < img.GetWidth(); ++w) {
            unsigned char r = static_cast<unsigned char>(img.GetPixel(h, w).red * 255.00);
            unsigned char g = static_cast<unsigned char>(img.GetPixel(h, w).green * 255.00);
            unsigned char b = static_cast<unsigned char>(img.GetPixel(h, w).blue * 255.00);
            unsigned char pixel[] = {b, g, r};
            file.write(reinterpret_cast<char*>(pixel), 3);
        }
        file.write(reinterpret_cast<char*>(padding), size_of_padding);
    }
    file.close();
    return;
}

Image BMP::Read(std::string path_to_file) {
    std::ifstream file;
    file.open(path_to_file);
    if (!file.is_open()) {
        throw std::runtime_error("File reading error");
    } else {
        unsigned char infoHeader[INFOHEADERSIZE];
        file.read(reinterpret_cast<char*>(infoHeader), INFOHEADERSIZE);
        unsigned char DIBHeader[DIBHEADERSIZE];
        file.read(reinterpret_cast<char*>(DIBHeader), DIBHEADERSIZE);
        if (infoHeader[0] != 'B' || infoHeader[1] != 'M') {
            throw std::runtime_error("Wrong format");
        } else {
            size_t width = DIBHeader[4] + (DIBHeader[5] << 8) + (DIBHeader[6] << 16) + (DIBHeader[7] << 24);
            size_t height = DIBHeader[8] + (DIBHeader[9] << 8) + (DIBHeader[10] << 16) + (DIBHeader[11] << 24);
            Image image(height, width);
            int size_of_padding = (4 - (3 * image.GetWidth()) % 4) % 4;
            for (size_t i = 0; i < image.GetHeight(); ++i) {
                for (size_t j = 0; j < image.GetWidth(); ++j) {
                    unsigned char pixel[3];
                    file.read(reinterpret_cast<char*>(pixel), 3);
                    double r = static_cast<double>(pixel[2]) / 255.00;
                    double g = static_cast<double>(pixel[1]) / 255.00;
                    double b = static_cast<double>(pixel[0]) / 255.00;
                    image.SetPixel(i, j, Color(r, g, b));
                }
                file.ignore(size_of_padding);
            }
            file.close();
            return image;
        }
    }
}

Image::Image(const size_t& height, const size_t& width) : width_(width), height_(height) {
    for (size_t i = 0; i < height_; ++i) {
        pixel_matrix_.push_back({});
        for (size_t j = 0; j < width; ++j) {
            pixel_matrix_[i].push_back(Color(0, 0, 0));
        }
    }
}

Image::Image(const ImageMatrix& init_mat) : pixel_matrix_(init_mat) {}

ImageMatrix& Image::GetImage() {
    return pixel_matrix_;
}

Color& Image::GetPixel(const size_t& height, const size_t& width) {
    return pixel_matrix_[height][width];
}

void Image::SetPixel(const size_t& height, const size_t& width, const Color& pixel) {
    pixel_matrix_[height][width] = pixel;
}

size_t Image::GetWidth() {
    return width_;
}

size_t Image::GetHeight() {
    return height_;
}

void Image::SetImageMatrix(ImageMatrix& image_matrix) {
    width_ = image_matrix[0].size();
    height_ = image_matrix.size();
    pixel_matrix_ = image_matrix;
}

const ImageMatrix& Image::GetImageMatrix() {
    return pixel_matrix_;
}

Color::Color(double r, double g, double b) : red(r), green(g), blue(b) {}

Color Color::operator+(Color other) {
    return Color(red + other.red, green + other.green, blue + other.blue);
}

bool Color::operator==(const Color& other) const {
    return (red == other.red && green == other.green && blue == other.blue);
}

Color& Color::operator+=(Color other) {
    *this = *this + other;
    return *this;
}

InformationHeader::InformationHeader(const int32_t& file_size) {
    header[0] = 'B';
    header[1] = 'M';
    header[2] = file_size;
    header[3] = file_size >> 8;
    header[4] = file_size >> 16;
    header[5] = file_size >> 24;
    header[6] = 0;
    header[7] = 0;
    header[8] = 0;
    header[9] = 0;
    header[10] = INFOHEADERSIZE + DIBHEADERSIZE;
    header[11] = 0;
    header[12] = 0;
    header[13] = 0;
}

DIB_header::DIB_header(const int32_t& width, const int32_t& height) {
    header[0] = DIBHEADERSIZE;
    header[1] = 0;
    header[2] = 0;
    header[3] = 0;
    header[4] = width;
    header[5] = width >> 8;
    header[6] = width >> 16;
    header[7] = width >> 24;
    header[8] = height;
    header[9] = height >> 8;
    header[10] = height >> 16;
    header[11] = height >> 24;
    header[12] = 1;

    for (int j = 13; j < DIBHEADERSIZE; ++j) {
        if (j != 14) {
            header[j] = 0;
        } else {
            header[j] = 24;
        }
    }
};
