#ifndef TIME_HPP
#define TIME_HPP

#include <string>

class Time {
public:
    static int FromString(const std::string &timeStr, bool &success);

    static std::string ToString(int minutes);
};

#endif // TIME_HPP
