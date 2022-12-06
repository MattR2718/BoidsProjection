cmake_minimum_required(VERSION 3.22.0)

add_library(drawables drawable.cpp point.cpp line.cpp box.cpp vect.cpp camera.cpp drawableData.cpp window.cpp)

find_path(SFML_INCLUDES SFML/Graphics.hpp)
target_include_directories(drawables PUBLIC drawable.h point.h line.h vect.h camera.h drawableData.h window.h ${SFML_INCLUDES})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    imgui::imgui
    sfml-graphics sfml-system sfml-window
    ImGui-SFML::ImGui-SFML
    drawables
)