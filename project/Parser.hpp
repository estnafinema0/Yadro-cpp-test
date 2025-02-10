#pragma once

#include <string>
#include <vector>

namespace Yadro {

struct ClubConfig {
    int openTime; // in minutes
    int closeTime; // in minutes
    int numTables;
    int hourlyCost;
};

struct EventData {
    int time;
    int eventId;
    std::string ClientName;
    int TableNumber;
    std::string originalLine;
};


class Parser {
public:
    // Constructor: call readLines method
    explicit Parser(const std::string &filename);
    // Start parsing - check config lines (lines 1, 2, 3) and events lines (lines 4+)
    bool ExecuteLines(ClubConfig &config, std::vector<EventData> &events, std::string & errorLine);
private:
    std::vector<std::string> m_lines;
    // Read lines from file and save to lines
    bool readLines(const std::string &filename, std::string &errorLine);
};

}
