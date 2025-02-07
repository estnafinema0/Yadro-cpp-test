#ifndef HELPFORCHECKER_HPP
#define HELPFORCHECKER_HPP

#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> mysplit(const std::string &s)
{
    // split string for tokens by spaces
    std::istringstream iss(s);
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

#endif