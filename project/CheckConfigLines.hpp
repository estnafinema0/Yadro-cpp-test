#ifndef CHECKCONFIGLINES_HPP
#define CHECKCONFIGLINES_HPP

// Here are functions to check config lines: line 1, 2, 3 from file

#include <string>

#include "Parser.hpp"
#include "HelpFuncs.hpp"
#include "Time.hpp"

// Check if line is valid table count line and save it to config
bool checkTableCount(const std::string &line, ClubConfig &config, std::string &errorLine) {
    auto tokens = mysplit(line);
    if (tokens.size() != 1) {
        errorLine = line;
        return false;
    }
    if (!IntFromString(tokens[0], config.numTables)) {
        errorLine = line;
        return false;
    }
    if (config.numTables <= 0) {
        errorLine = line;
        return false;
    }
    return true;
}

// Check if line is valid operating hours line and save it to config
bool checkOperatingHours(const std::string &line, ClubConfig &config, std::string &errorLine) {
    auto tokens = mysplit(line);
    if (tokens.size() != 2) {
        errorLine = line;
        return false;
    }
    bool success = false;
    config.openTime = Time::FromString(tokens[0], success);
    if (!success) {
        errorLine = line;
        return false;
    }
    config.closeTime = Time::FromString(tokens[1], success);
    if (!success) {
        errorLine = line;
        return false;
    }
    if (config.openTime >= config.closeTime) {
        errorLine = line;
        return false;
    }
    return true;
}

// Check if line is valid hourly cost line and save it to config
bool checkHourlyCost(const std::string &line, ClubConfig &config, std::string &errorLine) {
    auto tokens = mysplit(line);
    if (tokens.size() != 1) {
        errorLine = line;
        return false;
    }
    if (!IntFromString(tokens[0], config.hourlyCost)) {
        errorLine = line;
        return false;
    }
    if (config.hourlyCost <= 0) {
        errorLine = line;
        return false;
    }
    return true;
}

// Check if line is valid event line and save it to event
bool checkEventLine(const std::string &line, EventData &event, std::string &errorLine) {
    auto tokens = mysplit(line);
    if (tokens.size() < 2) {
        errorLine = line;
        return false;
    }

    bool success = false;
    event.time = Time::FromString(tokens[0], success);
    if (!success) {
        errorLine = line;
        return false;
    }
    int eventId = 0;
    if (!IntFromString(tokens[1], eventId)) {
        errorLine = line;
        return false;
    }
    if (eventId < 1 || eventId > 4) {
        errorLine = line;
        return false;
    }

    event.eventId = eventId;
    int expectedParams = (eventId == 2) ? 2 : 1;
    if (tokens.size() != 2 + expectedParams) {
        errorLine = line;
        return false;
    }
    event.originalLine = line;
    event.params.clear();
    for (size_t i = 2; i < tokens.size(); i++) {
        event.params.push_back(tokens[i]);
    }
    return true;
}

#endif