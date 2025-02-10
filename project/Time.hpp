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
    inline std::optional<int> FromString(std::string_view timeStr);

    // Formats minutes in a string "HH:MM".
    inline std::string ToString(int totalMinutes);

} 
