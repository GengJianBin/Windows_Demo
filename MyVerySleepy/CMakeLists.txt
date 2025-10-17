cmake_minimum_required(VERSION 3.10)  # specy CMake the lowest version

# project name and language
project(PerfProfDemo VERSION 1.0 LANGUAGES CXX)

# set C/C++ standart
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("output directory: ${CMAKE_BINARY_DIR}")
# set global ouput directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  # static library output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # exe output directory

# include source code directory
add_subdirectory(src)

# include demo directory
add_subdirectory(demo)