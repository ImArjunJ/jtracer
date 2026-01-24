cmake_minimum_required(VERSION 4.2)
project(jtracer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_executable(jtracer src/main.cpp src/jtracer/ppm.cpp)