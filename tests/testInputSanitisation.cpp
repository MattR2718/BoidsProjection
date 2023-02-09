#include <regex>
#include <string>
#include <vector>
#include <array>

#include <gtest/gtest.h>

#include "sanitisation.h"

TEST(TrueTest, AlwaysTrue){
    EXPECT_EQ(1, 1);
}

TEST(RemoveInvalidCharacters, RemoveInvalidCharacters){
    std::vector<std::string> testInputs{
        "hello",
        "1234",
        "12.34",
        "1.42e-3",
        "1.23test"
    };

    std::vector<std::string> testOutput{
        "e",
        "1234",
        "12.34",
        "1.42e-3",
        "1.23e"
    };

    for(auto& in : testInputs){
        clean(in);
    }

    for(int i = 0; i < testInputs.size(); i++){
        EXPECT_EQ(testInputs[i], testOutput[i]);
    }
}