cmake_minimum_required(VERSION 3.30)
project(AdventOfCode202401)

set(CMAKE_CXX_STANDARD 20)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(AdventOfCode202401 main.cpp)
