#include "validation.h"

bool floatingPointConversionCheck(const std::string& str){
    std::regex ex("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");
    return std::regex_match(str, ex);
}

bool rangeCheck(const float& fl, const float min, const float max){
    return (fl <= max) && (fl >= min);
}
