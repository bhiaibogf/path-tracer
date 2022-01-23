cmake_minimum_required(VERSION 3.16)
project(path_tracer)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC src/*.cpp)

add_executable(path_tracer ${SRC})

#add_executable(test test/main.cpp)
