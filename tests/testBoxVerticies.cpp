#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>

#include <gtest/gtest.h>

#include "drawable.h"
#include "point.h"
#include "line.h"
#include "box.h"

TEST(TrueTest, AlwaysTrue){
    EXPECT_EQ(1, 1);
}

TEST(GenerateVerticies, O20){
    Box b = Box(0, 0, 0, 20, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{-10, -10, -10},
                                                            {-10, -10, 10},
                                                            {-10, 10, -10},
                                                            {-10, 10, 10},
                                                            {10, -10, -10},
                                                            {10, -10, 10},
                                                            {10, 10, -10},
                                                            {10, 10, 10}};
    EXPECT_EQ(expectedVerticies, verticies);
}


TEST(GenerateVerticies, M1020){
    Box b = Box(-10, -10, -10, 20, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{-20, -20, -20},
                                                            {-20, -20, 0},
                                                            {-20, 0, -20},
                                                            {-20, 0, 0},
                                                            {0, -20, -20},
                                                            {0, -20, 0},
                                                            {0, 0, -20},
                                                            {0, 0, 0}};
    EXPECT_EQ(expectedVerticies, verticies);
}


TEST(GenerateVerticies, 1001040){
    Box b = Box(10, 0, 10, 40, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{-10, -20, -10},
                                                                {-10, -20, 30},
                                                                {-10, 20, -10},
                                                                {-10, 20, 30},
                                                                {30, -20, -10},
                                                                {30, -20, 30},
                                                                {30, 20, -10},
                                                                {30, 20, 30}};
    EXPECT_EQ(expectedVerticies, verticies);
}


TEST(GenerateVerticies, 0M100020){
    Box b = Box(0, -100, 0, 20, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{-10, -110, -10},
                                                                {-10, -110, 10},
                                                                {-10, -90, -10},
                                                                {-10, -90, 10},
                                                                {10, -110, -10},
                                                                {10, -110, 10},
                                                                {10, -90, -10},
                                                                {10, -90, 10}};
    EXPECT_EQ(expectedVerticies, verticies);
}


TEST(GenerateVerticies, O200){
    Box b = Box(0, 0, 0, 200, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{-100, -100, -100},
                                                                {-100, -100, 100},
                                                                {-100, 100, -100},
                                                                {-100, 100, 100},
                                                                {100, -100, -100},
                                                                {100, -100, 100},
                                                                {100, 100, -100},
                                                                {100, 100, 100}};
    EXPECT_EQ(expectedVerticies, verticies);
}


TEST(GenerateVerticies, 2000M10025){
    Box b = Box(200, 0, -100, 25, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{188, -12, -112},
                                                                {188, -12, -88},
                                                                {188, 12 ,-112},
                                                                {188, 12 ,-88},
                                                                {212, -12, -112},
                                                                {212, -12, -88},
                                                                {212, 12 ,-112},
                                                                {212, 12 ,-88}};
    EXPECT_EQ(expectedVerticies, verticies);
}

TEST(GenerateVerticies, 08000200){
    Box b = Box(0, 800, 0, 200, 0, 0);
    std::vector<std::tuple<int, int, int>> verticies = b.generateVerticies();
    std::vector<std::tuple<int, int, int>> expectedVerticies = {{-100, 700, -100},
                                                                {-100, 700, 100},
                                                                {-100, 900, -100},
                                                                {-100, 900, 100},
                                                                {100, 700, -100},
                                                                {100, 700, 100},
                                                                {100, 900, -100},
                                                                {100, 900, 100}};
    EXPECT_EQ(expectedVerticies, verticies);
}