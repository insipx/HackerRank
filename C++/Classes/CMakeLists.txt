cmake_minimum_required(VERSION 3.6)
project(Classes)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Class.cpp)

add_executable(Classes ${SOURCE_FILES})