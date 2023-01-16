cmake_minimum_required(VERSION 3.22.0)
include(FetchContent)
project(BoidsProjection VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)


find_package(SFML CONFIG COMPONENTS graphics system window)
if(NOT SFML_FOUND)
    message("SFML NOT FOUND")
    FetchContent_Declare( SFML 
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(SFML)
endif(NOT SFML_FOUND)

find_package(imgui CONFIG)
if(NOT imgui_FOUND)
    message("IMGUI NOT FOUND") 
    FetchContent_Declare( imgui 
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(imgui)
    set(IMGUI_DIR "${imgui_SOURCE_DIR}")
    add_library(imgui
        "${imgui_SOURCE_DIR}/imgui.h"
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp")
    add_library(imgui::imgui ALIAS imgui)
    target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
endif(NOT imgui_FOUND)

find_package(ImGui-SFML CONFIG)
if(NOT ImGui-SFML_FOUND)
    message("IMGUI-SFML NOT FOUND")
    set(IMGUI_SFML_FIND_SFML OFF)
    FetchContent_Declare( imgui-sfml 
        GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml
        GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(imgui-sfml)
endif(NOT ImGui-SFML_FOUND)

find_package(implot CONFIG)
if(NOT implot_FOUND)
    message("IMPLOT NOT FOUND")
    FetchContent_Declare(ImPlot
        GIT_REPOSITORY https://github.com/epezent/implot
        GIT_TAG origin/master
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(ImPlot)
    add_library(implot
        "${implot_SOURCE_DIR}/implot.cpp"
        "${implot_SOURCE_DIR}/implot_items.cpp"
        "${imgui_SOURCE_DIR}/imgui.h"
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        
        )
    add_library(implot::implot ALIAS implot)
    target_include_directories(implot PUBLIC "${implot_SOURCE_DIR}" "${imgui_SOURCE_DIR}")
endif(NOT implot_FOUND)

add_subdirectory(src)
add_subdirectory(tests)


#TODO imgui.h not being found by executable, sf::uint8 not kniwn>