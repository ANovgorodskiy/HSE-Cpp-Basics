cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(image_processor
    image_processor.cpp
        Image.cpp
        Image.h
        Filters.cpp
        Filters.h
        ArgumentParser.cpp
        ArgumentParser.h
        FilterController.cpp
        FilterController.h
        help.h)


add_catch(image_test
        Image.cpp
        test_image.cpp)

add_catch(filter_controller_test
        FilterController.cpp
        Filters.cpp
        ArgumentParser.cpp
        test_filter_controller.cpp)

add_catch(argument_parser_test
        ArgumentParser.cpp
        test_argparser.cpp)