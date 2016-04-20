project("aelib")

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg")
include_directories(include)

add_library(aelib SHARED src/string_manipulation.cpp)
