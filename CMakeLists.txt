cmake_minimum_required(VERSION 3.16)
project(path_tracer)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC src/*.cpp)

include_directories(./third_party)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(path_tracer ${SRC})

target_link_libraries(path_tracer ${CMAKE_THREAD_LIBS_INIT} ${OpenCV_LIBRARIES})

#add_executable(test test/main.cpp)

#add_subdirectory(Assignment7)
