#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <optional>
#include <cstdlib>

namespace Yadro {
namespace Util {
    
std::vector<std::string> splitString(const std::string &str) {
    // split string for tokens by spaces
    std::istringstream inputStream(str);
    std::vector<std::string> tokens;
    std::string token;
    while (inputStream >> token)
        tokens.push_back(token);
    return tokens;
}

inline std::optional<int> FromString(const std::string &str) {
    char* endPtr = nullptr;
    long value = std::strtol(str.c_str(), &endPtr, 10);
    if (*endPtr != '\0')
        return std::nullopt;
    return static_cast<int>(value);
}

}
}