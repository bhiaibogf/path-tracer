cmake_minimum_required(VERSION 3.16)
project(path_tracer)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC src/*.cpp)

include_directories(./third_party)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

add_library(tiny_obj_loader_lib third_party/tinyobjloader/tiny_obj_loader.cc)
add_library(tiny_xml_lib third_party/tinyxml2/tinyxml2.cpp)
add_library(stb_lib third_party/stb/stb.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(path_tracer ${SRC})

target_link_libraries(path_tracer ${OpenCV_LIBRARIES} OpenMP::OpenMP_CXX tiny_obj_loader_lib tiny_xml_lib stb_lib)
