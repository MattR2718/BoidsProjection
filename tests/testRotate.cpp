#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>

#include <gtest/gtest.h>

#include "drawable.h"
#include "point.h"

#define PI 3.14159

TEST(TrueTest, AlwaysTrue){
    EXPECT_EQ(1, 1);
}

TEST(Rotate, RotateX0){
    float tx = 0, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX90){
    float tx = 90, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX180){
    float tx = 180, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX360){
    float tx = 360, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateX45){
    float tx = 45, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(100, 0, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {100,0,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY0){
    float tx = 0, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY90){
    float tx = 0, ty = 90, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY180){
    float tx = 0, ty = 180, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY360){
    float tx = 0, ty = 360, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateY45){
    float tx = 0, ty = 45, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 100, 0, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,-100,0};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ0){
    float tx = 0, ty = 0, tz = 0;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ90){
    float tx = 0, ty = 0, tz = 90;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ180){
    float tx = 0, ty = 0, tz = 180;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ360){
    float tx = 0, ty = 0, tz = 360;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}

TEST(Rotate, RotateZ45){
    float tx = 0, ty = 0, tz = 45;
    auto degToRad = [](float angle){
            return float(angle) * PI / 180.f;
    };
    std::map<std::string, float> trigFunctions = {{"sx", std::sin(degToRad(tx))},
                                                    {"sy", std::sin(degToRad(ty))},
                                                    {"sz", std::sin(degToRad(tz))},
                                                    {"cx", std::cos(degToRad(tx))},
                                                    {"cy", std::cos(degToRad(ty))},
                                                    {"cz", std::cos(degToRad(tz))}};
    Point p(0, 0, 100, 0, 0);
    p.rotAll(tx, ty, tz, trigFunctions, 0);
    std::tuple<int, int, int> exp = {0,0,100};
    EXPECT_EQ(exp, p.getPXYZ());
}