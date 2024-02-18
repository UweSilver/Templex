cmake_minimum_required(VERSION 3.26)
project(Templex)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_executable(Templex ${SOURCES})