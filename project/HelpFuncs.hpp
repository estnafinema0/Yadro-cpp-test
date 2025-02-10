#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace Yadro {
namespace Util {
    
std::vector<std::string> splitString(const std::string &str) {
    // split string for tokens by spaces
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token)
        tokens.push_back(token);

    return tokens;
}

bool IntFromString(const std::string &s, int &value) {
    // returns true if string is a valid integer
    char* endPtr = nullptr;
    long val = std::strtol(s.c_str(), &endPtr, 10);
    if (*endPtr != '\0') {
        return false;
    }
    
    value = static_cast<int>(val);
    return true;
}

}
}