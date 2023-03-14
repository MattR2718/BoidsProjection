#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>

#include <gtest/gtest.h>

#include "drawable.h"
#include "point.h"

#define PI 3.14159

float tx = 0, ty = 0, tz = 0;
auto degToRad = [](float angle){
        return float(angle) * PI / 180.f;
};

std::map<std::string, float> trigFunctions = {{"sx", (float)std::sin(degToRad(tx))},
                                                {"sy", (float)std::sin(degToRad(ty))},
                                                {"sz", (float)std::sin(degToRad(tz))},
                                                {"cx", (float)std::cos(degToRad(tx))},
                                                {"cy", (float)std::cos(degToRad(ty))},
                                                {"cz", (float)std::cos(degToRad(tz))}};

void setTrigValues(float x, float y, float z){
    trigFunctions.at("sx") = std::sin(degToRad(x));
    trigFunctions.at("sy") = std::sin(degToRad(y));
    trigFunctions.at("sz") = std::sin(degToRad(z));
    trigFunctions.at("cx") = std::cos(degToRad(x));
    trigFunctions.at("cy") = std::cos(degToRad(y));
    trigFunctions.at("cz") = std::cos(degToRad(z));
}

TEST(TrueTest, AlwaysTrue){
    EXPECT_EQ(1, 1);
}

TEST(Rotate, RotateX0){
    tx = 0; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX90){
    tx = 90; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX180){
    tx = 180; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX360){
    tx = 360; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX45){
    tx = 45; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY0){
    tx = 0; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY90){
    tx = 0; ty = 90; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY180){
    tx = 0; ty = 180; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY360){
    tx = 0; ty = 360; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY45){
    tx = 0; ty = 45; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ0){
    tx = 0; ty = 0; tz = 0;
    setTrigValues(tx, ty, tz);
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ90){
    tx = 0; ty = 0; tz = 90;
    setTrigValues(tx, ty, tz);
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ180){
    tx = 0; ty = 0; tz = 180;
    setTrigValues(tx, ty, tz);
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ360){
    tx = 0; ty = 0; tz = 360;
    setTrigValues(tx, ty, tz);
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ45){
    tx = 0; ty = 0; tz = 45;
    setTrigValues(tx, ty, tz);
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}