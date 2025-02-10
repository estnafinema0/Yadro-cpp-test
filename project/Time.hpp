#pragma once
#include <string_view>
#include <string>
#include <optional>
#include <sstream>
#include <iomanip>

namespace Time {

    // Parses the time in the "HH:MM" format from a string.
    // If parsing is successful, returns the number of minutes since midnight,
    // otherwise it returns std::nullptr.
    inline std::optional<int> FromString(std::string_view timeStr) {
        if (timeStr.size() != 5 || timeStr[2] != ':')
            return std::nullopt;
        std::string_view hourPart = timeStr.substr(0, 2);
        std::string_view minutePart = timeStr.substr(3, 2);
        char* endPtr = nullptr;
        std::string hourStr(hourPart);
        long hours = std::strtol(hourStr.c_str(), &endPtr, 10);
        if (*endPtr != '\0')
            return std::nullopt;
        std::string minuteStr(minutePart);
        endPtr = nullptr;
        long minutes = std::strtol(minuteStr.c_str(), &endPtr, 10);
        if (*endPtr != '\0')
            return std::nullopt;
        if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59)
            return std::nullopt;
        return static_cast<int>(hours * 60 + minutes);
    }

    // Formats minutes in a string "HH:MM".
    inline std::string ToString(int totalMinutes);

} 
