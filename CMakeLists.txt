cmake_minimum_required(VERSION 3.28)
project(zeiss)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(zeiss_integral_image
        src/integral_image_input_examples.cpp
        src/sequential_integral_image.cpp
        src/parallel_integral_image.cpp
        src/gpu_integral_image.cpp
)
target_include_directories(zeiss_integral_image PUBLIC include)
target_link_libraries(zeiss_integral_image ${OpenCV_LIBS})

add_executable(zeiss_integral_image_tests tests/tests.cpp)
target_include_directories(zeiss_integral_image_tests PUBLIC doctest)
target_link_libraries(zeiss_integral_image_tests zeiss_integral_image)

add_executable(zeiss_integral_image_timer tests/timer.cpp)
target_link_libraries(zeiss_integral_image_timer zeiss_integral_image)