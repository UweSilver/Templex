cmake_minimum_required(VERSION 3.26)
project(Templex)

set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_API)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(Templex ${SOURCES})