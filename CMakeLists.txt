cmake_minimum_required(VERSION 3.22.0)
project(BoidsProjection VERSION 0.1.0)

find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
include_directories(src)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)