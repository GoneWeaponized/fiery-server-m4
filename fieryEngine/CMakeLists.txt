cmake_minimum_required(VERSION 4.3)
project("fieryEngine")

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(${CMAKE_PROJECT_NAME} SHARED
        GeoOp.hpp
        test.cpp
)
