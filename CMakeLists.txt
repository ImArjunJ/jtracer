cmake_minimum_required(VERSION 4.2)
project(jtracer)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

add_subdirectory(deps)
add_executable(jtracer src/main.cpp src/jtracer/ppm.cpp src/jtracer/window.cpp)
target_link_libraries(jtracer glfw glad_gl_core_43 imgui_glfw)