#include <string>
struct Helper{
    std::string readme = "Welcome to image processor version 1.1\n"
        "Imprtant! Image processsor process only images in bmp format\n"
        "Available commands:\n"
        "-blur with double parameter sigma - Gaussian Blur\n"
        "-HIBlur with no parameters - Horizontal Integral Blur\n"
        "-gs with no parameters - Grayscale filter\n"
        "-sharp with no parameters - sharps image\n"
        "-crop with two integer parameter(height and width) - crops image\n"
        "-neg with no parameters - makes negative image representation\n"
        "-edge with double parameter(threshold) - does edge detection according to threshold\n";
};