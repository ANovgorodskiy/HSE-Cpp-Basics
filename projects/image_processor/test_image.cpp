#include <catch.hpp>
#include "Image.h"
#include <iostream>
#include <cstring>
TEST_CASE("Read"){
    std::string path_to_test_image = "../projects/image_processor/images_for_testing/example.bmp";
    Image image_readed = BMP::Read(path_to_test_image);

    REQUIRE(image_readed.GetWidth() == 2048);
    REQUIRE(image_readed.GetHeight() == 2048);
}

TEST_CASE("Incorrect path read"){
    std::string incorrect_path = "/...abracadabre";
    try {
        auto image = BMP::Read(incorrect_path);
    }
    catch(std::runtime_error const& exception){
        REQUIRE(std::strcmp(exception.what(), "File reading error") == 0);
    }

}

TEST_CASE("Incorrect image type"){
    std::string incorrect_image_path = "../projects/image_processor/images_for_testing/wrong_type_image.jpg";
    try {
        auto image = BMP::Read(incorrect_image_path);
    }
    catch(std::runtime_error const& exception){
        REQUIRE(std::strcmp(exception.what(), "Wrong format") == 0);
    }
}

TEST_CASE("Write"){
    std::string image_path = "../projects/image_processor/images_for_testing/example.bmp";
    auto image =  BMP::Read(image_path);
    BMP::Write(image_path, image);
    auto image_to_compare = BMP::Read(image_path);
    REQUIRE(image_to_compare.GetImageMatrix() == image.GetImageMatrix());

}