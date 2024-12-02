cmake_minimum_required(VERSION 3.24)

project(sdl++)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(deps)

add_library(sdl++
    error.cpp
    sdl.cpp
)
target_include_directories(sdl++ PUBLIC include)
target_link_libraries(sdl++ PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)