#include "Time.hpp"

int Time::FromString(const std::string &timeStr, bool &success){
    if (timeStr.size() != 5 || timeStr[2] != ':') {
        success = false;
        return 0;
    }

    char *endPtr = nullptr;
    long hours = std::strtol(timeStr.substr(0, 2).c_str(), &endPtr, 10);
    if (*endPtr != '\0') {
        success = false;
        return 0;
    }

    endPtr = nullptr;
    long minutes = std::strtol(timeStr.substr(3, 2).c_str(), &endPtr, 10);
    if (*endPtr != '\0') {
        success = false;
        return 0;
    }

    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        success = false;
        return 0;
    }

    success = true;
    return static_cast<int>(hours * 60 + minutes);
}

std::string Time::ToString(int minutes){

}