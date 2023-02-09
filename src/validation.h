#ifndef VALIDATION_H
#define VALIDATION_H
#include <string>
#include <regex>

bool floatingPointConversionCheck(const std::string& str);
bool rangeCheck(const float& fl, const float min = -10, const float max = 10);

#endif