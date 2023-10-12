cmake_minimum_required(VERSION 3.26)
project (SnazeGame VERSION 1.0.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
add_executable(sudoku main.cpp
                      graph.cpp
                      fileio.cpp)
