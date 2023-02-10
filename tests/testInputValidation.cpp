#include <regex>
#include <string>
#include <vector>
#include <array>

#include <gtest/gtest.h>

#include "validation.h"

TEST(TrueTest, AlwaysTrue){
    EXPECT_EQ(1, 1);
}

TEST(FloatingPointConversionCheck, ValidConvertions){
    std::vector<std::string> testInputs{
        "0.5",
        "0.625",
        "10.5",
        "10",
        "0.000005",
        "0.5089695129344164",
        "0.07174329054775652",
        "0.7576474741201431"
    };
    for(auto& input : testInputs){
        bool out = floatingPointConversionCheck(input);
        EXPECT_EQ(out, true);
    }
}

TEST(FloatingPointConversionCheck, InvalidConvertions){
    std::vector<std::string> testInputs{
        "number",
        "",
        "a",
        "10.0a",
        "10.10.10",
        "1.625\n",
        "1.625 ",
        "0.0  56",
        "0.56 a",
        "0.888 0.898",
        "1.23-3"
    };
    for(auto& input : testInputs){
        bool out = floatingPointConversionCheck(input);
        EXPECT_EQ(out, false);
    }
}

TEST(FloatingPointRangeCheck, ValidRanges){
    std::vector<std::array<float, 3>> testInputs{
        {0, -10, 10},
        {10, -10, 10},
        {0.5, -1, 1},
        {0.87654, 0.87653, 0.87655},
        {100, 90, 1000},
        {-100, -110, -90},
        {1.41, 0.667, 3.141}
    };
    for(auto& input : testInputs){
        bool out = rangeCheck(input[0], input[1], input[2]);
        EXPECT_EQ(out, true);
    }
}

TEST(FloatingPointRangeCheck, InvalidRanges){
    std::vector<std::array<float, 3>> testInputs{
        {10, 0, 1},
        {1, 0, 0.5},
        {-100, -110, -105},
        {0.065, 0.0644, 0.0645},
        {-1, 0, 1},
        {200, 300, 450},
        {0.67654462, 0.88633739, 0.8939877},
        {0.6125, -10, 0.612499}
    };
    for(auto& input : testInputs){
        bool out = rangeCheck(input[0], input[1], input[2]);
        EXPECT_EQ(out, false);
    }
}