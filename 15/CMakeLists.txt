cmake_minimum_required(VERSION 3.30)
project(AdventOfCode2024_15)

set(CMAKE_CXX_STANDARD 20)

add_executable(AdventOfCode2024_15 main.cpp
        ../shared/Vector2.cpp
        ../shared/Vector2.h
        ../shared/Direction.h
        ../shared/Grid2.h)
