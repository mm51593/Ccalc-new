cmake_minimum_required(VERSION 3.20)
project(Ccalc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Ccalc test/test_reader.cpp src/reader/reader.cpp)
