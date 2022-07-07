add_library(contrib_catch_main
  contrib/catch/catch_main.cpp ../projects/image_processor/Image.cpp
        ../projects/image_processor/Image.h
        ../projects/image_processor/Filters.h
        ../projects/image_processor/FilterController.h
        ../projects/image_processor/FilterController.cpp
        ../projects/image_processor/test_image.cpp
        ../projects/image_processor/test_filter_controller.cpp
        ../projects/image_processor/test_argparser.cpp)

target_include_directories(contrib_catch_main
  PUBLIC contrib/catch)
