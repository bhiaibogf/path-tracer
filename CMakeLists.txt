cmake_minimum_required(VERSION 3.16)
project(path_tracer)

set(CMAKE_CXX_STANDARD 20)

find_package(Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)

add_library(tiny_obj_loader_lib third_party/tinyobjloader/tiny_obj_loader.cc)
add_library(tiny_xml_lib third_party/tinyxml2/tinyxml2.cpp)

add_library(tiny_exr_miniz_lib third_party/tinyexr/miniz/miniz.c)
add_library(tiny_exr_lib third_party/tinyexr/tinyexr.cc)
target_link_libraries(tiny_exr_lib tiny_exr_miniz_lib)

add_library(stb_lib third_party/stb/stb.c)

file(GLOB_RECURSE SRC src/*.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(path_tracer ${SRC})

target_include_directories(path_tracer PUBLIC third_party)
target_link_libraries(path_tracer Eigen3::Eigen OpenMP::OpenMP_CXX tiny_obj_loader_lib tiny_xml_lib tiny_exr_lib stb_lib)
