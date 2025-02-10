#pragma once

#include <string>
#include <iomanip>
#include <sstream>
#include <optional>
#include <string_view>
#include <cstdlib>

namespace Time {

inline std::optional<int> FromString(std::string_view timeStr) {
    if (timeStr.size() != 5 || timeStr[2] != ':')
        return std::nullopt;
    
    std::string hoursStr{timeStr.substr(0, 2)};
    std::string minutesStr{timeStr.substr(3, 2)};
    
    char* endPtr = nullptr;
    long hours = std::strtol(hoursStr.c_str(), &endPtr, 10);
    if (*endPtr != '\0')
        return std::nullopt;
    endPtr = nullptr;
    long minutes = std::strtol(minutesStr.c_str(), &endPtr, 10);
    if (*endPtr != '\0')
        return std::nullopt;
    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
        return std::nullopt;
    return static_cast<int>(hours * 60 + minutes);
}
    
inline std::string ToString(int totalMinutes) {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

};