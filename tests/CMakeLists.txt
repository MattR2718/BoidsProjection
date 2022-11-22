cmake_minimum_required(VERSION 3.22.0)


find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test testRotate.cpp)

target_link_libraries(test
    GTest::GTest 
    GTest::Main
    drawables
)