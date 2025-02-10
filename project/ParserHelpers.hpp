#pragma once

#include <string>
#include <vector>
#include "Parser.hpp"
#include "Utils.hpp"
#include "Time.hpp"

namespace Yadro {

inline bool parseTableCount(const std::string &line, ClubConfig &config, std::string &errorLine) {
    auto tokens = Util::splitString(line);
    if (tokens.size() != 1) {
        errorLine = line;
        return false;
    }
    auto maybeNum = Util::FromString(tokens[0]);
    if (!maybeNum.has_value()) {
        errorLine = line;
        return false;
    }
    config.numTables = maybeNum.value();
    if (config.numTables <= 0) {
        errorLine = line;
        return false;
    }
    return true;
}

inline bool parseOperatingHours(const std::string &line, ClubConfig &config, std::string &errorLine) {
    auto tokens = Util::splitString(line);
    if (tokens.size() != 2) {
        errorLine = line;
        return false;
    }
    auto maybeOpen = Time::FromString(tokens[0]);
    if (!maybeOpen.has_value()) {
        errorLine = line;
        return false;
    }
    config.openTime = maybeOpen.value();
    auto maybeClose = Time::FromString(tokens[1]);
    if (!maybeClose.has_value()) {
        errorLine = line;
        return false;
    }
    config.closeTime = maybeClose.value();
    if (config.openTime >= config.closeTime) {
        errorLine = line;
        return false;
    }
    return true;
}

inline bool parseHourlyCost(const std::string &line, ClubConfig &config, std::string &errorLine) {
    auto tokens = Util::splitString(line);
    if (tokens.size() != 1) {
        errorLine = line;
        return false;
    }
    auto maybeCost = Util::FromString(tokens[0]);
    if (!maybeCost.has_value()) {
        errorLine = line;
        return false;
    }
    config.hourlyCost = maybeCost.value();
    if (config.hourlyCost <= 0) {
        errorLine = line;
        return false;
    }
    return true;
}

inline bool parseEvent(const std::string &line, EventData &event, ClubConfig &config, std::string &errorLine) {
    auto tokens = Util::splitString(line);
    if (tokens.size() < 2) {
        errorLine = line;
        return false;
    }
    auto maybeTime = Time::FromString(tokens[0]);
    if (!maybeTime.has_value()) {
        errorLine = line;
        return false;
    }
    event.time = maybeTime.value();
    auto maybeEventId = Util::FromString(tokens[1]);
    if (!maybeEventId.has_value()) {
        errorLine = line;
        return false;
    }
    int eventId = maybeEventId.value();
    if (eventId < 1 || eventId > 4) {
        errorLine = line;
        return false;
    }

    event.eventId = eventId;
    size_t expectedParams = (eventId == 2) ? 2 : 1;
    if (tokens.size() != 2 + expectedParams) {
        errorLine = line;
        return false;
    }
    event.originalLine = line;
    event.TableNumber = -1;
    event.ClientName = tokens[2];
    if (eventId == 2) {
        auto maybeTable = Util::FromString(tokens[3]);
        if (!maybeTable.has_value()) {
            errorLine = line;
            return false;
        }
        event.TableNumber = maybeTable.value();
        if (event.TableNumber < 1 || event.TableNumber > config.numTables) {
            errorLine = line;
            return false;
        }
    }
    return true;
}

}
