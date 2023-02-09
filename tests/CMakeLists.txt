cmake_minimum_required(VERSION 3.22.0)

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(testRotate testRotate.cpp)

target_link_libraries(testRotate
    GTest::GTest 
    GTest::Main
    drawables
)

add_executable(testBoxVerticies testBoxVerticies.cpp)

target_link_libraries(testBoxVerticies
    GTest::GTest 
    GTest::Main
    drawables
)

add_executable(testInputValidation testInputValidation.cpp)

target_link_libraries(testInputValidation
    GTest::GTest 
    GTest::Main
    drawables
)

add_executable(testInputSanitisation testInputSanitisation.cpp)

target_link_libraries(testInputSanitisation
    GTest::GTest 
    GTest::Main
    drawables
)