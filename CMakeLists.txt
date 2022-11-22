cmake_minimum_required(VERSION 3.0.0)
project(BoidsProjection VERSION 0.1.0)

# SET(CMAKE_TOOLCHAIN_FILE "C:\\Users\\matth\\vcpkg\\scripts\\buildsystem\\vcpkg.cmake")

set(SFML_DIR "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\sfml")
find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
include_directories(${SFML_INCLUDE_DIR})

set(imgui_DIR "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\imgui")
find_package(imgui CONFIG REQUIRED)
set(ImGui-SFML_DIR "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\ImGui-SFML")
find_package(ImGui-SFML CONFIG REQUIRED)

set(GTest_DIR "C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\share\\gtest")
find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

# link_directories ("C:\\Users\\matth\\vcpkg\\installed\\x86-windows\\debug\\bin") 

add_executable(${PROJECT_NAME} src/main.cpp src/drawable.cpp src/point.cpp src/line.cpp)
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

# if (WIN32)
# copy the .dll file to the same folder as the executable
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     $<TARGET_FILE_DIR:sfml-window>
#     $<TARGET_FILE_DIR:${PROJECT_NAME}>)
# endif()