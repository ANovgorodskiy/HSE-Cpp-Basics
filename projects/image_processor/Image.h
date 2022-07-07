#pragma once
#include <vector>
#include <string>
#include <fstream>
const int INFOHEADERSIZE = 14;
const int DIBHEADERSIZE = 40;
struct DIB_header {
    unsigned char header[DIBHEADERSIZE];
    int32_t dib_header_size = DIBHEADERSIZE;
    int32_t image_width;
    int32_t image_height;
    int16_t planes = 1;
    int16_t bits_for_pixel = 24;
    int32_t compression = 0;
    int32_t image_size = 0;
    int32_t x_pix_per_meter = 0;
    int32_t y_pix_per_meter = 0;
    int32_t colors_in_ct = 0;
    int32_t important_colors = 0;
    DIB_header(const int32_t& width, const int32_t& height);
};

struct InformationHeader {
    unsigned char header[INFOHEADERSIZE];
    InformationHeader(const int32_t& filesize);
};

struct Color {
    double red = 0;
    double green = 0;
    double blue = 0;
    Color(double r, double g, double b);
    Color operator+(Color other);
    bool operator==(const Color& other) const;
    template <typename T>
    Color operator*(T num);
    Color& operator+=(Color other);
};
template <typename T>
Color Color::operator*(T num) {
    return Color(red * num, green * num, blue * num);
}

using ImageMatrix = std::vector<std::vector<Color>>;

class Image {
public:
    Image(const size_t& height, const size_t& width);
    Image(const ImageMatrix&);
    ImageMatrix& GetImage();
    void SetImageMatrix(ImageMatrix& image_matrix);
    Color& GetPixel(const size_t& height, const size_t& width);
    const ImageMatrix& GetImageMatrix();
    void SetPixel(const size_t& height, const size_t& width, const Color& pixel);
    size_t GetWidth();
    size_t GetHeight();

private:
    size_t width_ = 0;
    size_t height_ = 0;
    ImageMatrix pixel_matrix_;
};

class BMP {
public:
    static Image Read(std::string path_to_file);
    static void Write(std::string path_to_file, Image& img);
};