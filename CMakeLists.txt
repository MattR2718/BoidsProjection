cmake_minimum_required(VERSION 3.0.0)
project(BoidsProjection VERSION 0.1.0)

list(APPEND CMAKE_PREFIX_PATH "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\sfml")
find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
include_directories(${SFML_INCLUDE_DIR})

list(APPEND CMAKE_PREFIX_PATH "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\imgui")
list(APPEND CMAKE_PREFIX_PATH "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\ImGui-SFML")
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable(BoidsProjection src/main.cpp src/drawable.cpp src/point.cpp)

target_link_libraries(${PROJECT_NAME}
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
)