cmake_minimum_required(VERSION 3.10)  # specy CMake the lowest version

# project name and language
project(CMakeToolProject VERSION 1.0 LANGUAGES CXX)

# set C/C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add submodule
add_subdirectory(src)
