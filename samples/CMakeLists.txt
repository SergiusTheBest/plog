cmake_minimum_required(VERSION 2.7)

project(sample)

include_directories(${CMAKE_SOURCE_DIR}/../include)
add_executable(sample sample.cpp)