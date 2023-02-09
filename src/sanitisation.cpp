#include "sanitisation.h"

void clean(std::string& str){
    std::regex ex("[^0-9\\.\\+\\-e]");

    std::stringstream result;
    std::regex_replace(std::ostream_iterator<char>(result), str.begin(), str.end(), ex, "");

    str = result.str();
}

void sanitise(std::string& str){
    std::regex("[a-zA-Z]");
}