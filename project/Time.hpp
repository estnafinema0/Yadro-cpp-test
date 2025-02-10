#ifndef TIME_HPP
#define TIME_HPP

#include <string>

namespace Time {
    int FromString(const std::string &timeStr, bool &success);

    std::string ToString(int totalMinutes) {
        int hours = totalMinutes / 60;
        int minutes = totalMinutes % 60;
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << hours << ":"
            << std::setw(2) << std::setfill('0') << minutes;
        return oss.str();
    }

};

#endif
