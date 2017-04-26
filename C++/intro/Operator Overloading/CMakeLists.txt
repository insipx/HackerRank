cmake_minimum_required(VERSION 3.6)
project(Operator_Overloading)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Operator_Overloading ${SOURCE_FILES})