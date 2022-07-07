#include <catch.hpp>
#include <cstring>
#include "FilterController.h"

TEST_CASE("Filter creation") {
    FilterParams blur;
    blur.name = "-blur";
    blur.params.push_back("2");
    FilterParams gs;
    gs.name = "-gs";
    auto blur_ptr = CreateFilter(blur);
    auto gs_ptr = CreateFilter(gs);
    auto vec = CreateFilters({blur, gs});
}

TEST_CASE("Creation filter with wrong name"){
    FilterParams wrong_name_filter;
    wrong_name_filter.name = "-greyskal";
    try{
        auto filter = CreateFilter(wrong_name_filter);
    } catch (std::runtime_error const& exception){
        REQUIRE(std::strcmp(exception.what(), "No filter named -greyskal") == 0);
    }
}

TEST_CASE("Creation filter with wrong parameter"){
    FilterParams filter;
    filter.name = "-gs";
    filter.params.push_back("12");
    try{
        auto filter_ptr = CreateFilter(filter);
    } catch (std::runtime_error const& exception){
        REQUIRE(std::strcmp(exception.what(), "Grayscale has no parameters") == 0);
    }
}

