cmake_minimum_required(VERSION 3.22.0)
project(BoidsProjection VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

if(WIN32)
    find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
    find_package(imgui CONFIG REQUIRED)
    find_package(ImGui-SFML CONFIG REQUIRED)
    find_package(implot CONFIG REQUIRED)
endif (WIN32)

add_subdirectory(src)
add_subdirectory(tests)