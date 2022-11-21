cmake_minimum_required(VERSION 3.0.0)
project(BoidsProjection VERSION 0.1.0)

SET(CMAKE_TOOLCHAIN_FILE "C:\\Users\\matth\\vcpkg\\scripts\\buildsystem\\vcpkg.cmake")

find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
include_directories(${SFML_INCLUDE_DIR})

find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/drawable.cpp src/point.cpp)
add_executable(test tests/testRotate.cpp src/drawable.cpp src/point.cpp)

target_link_libraries(${PROJECT_NAME}
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
)

target_link_libraries(test
    GTest::GTest 
    GTest::Main
)