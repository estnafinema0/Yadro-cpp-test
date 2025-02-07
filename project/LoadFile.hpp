#ifndef LOADFILE_HPP
#define LOADFILE_HPP

#include <string>
#include <vector>

struct ClubConfig {
    int openTime;
    int closeTime;
    int numTables;
    int hourlyCost;
};

struct EventData {
    int time;
    int eventId;
    std::vector<std::string> params;
    std::string originalLine;
};


class FileParser {
public:
    // Constructor: call readLines method
    explicit FileParser(const std::string &filename);
    // Start parsing - check config lines (lines 1, 2, 3) and events lines (lines 4+)
    bool Start(ClubConfig &config, std::vector<EventData> &events, std::string & errorLine);
private:
    std::vector<std::string> lines;
    // Read lines from file and save to lines
    bool readLines(const std::string &filename, std::string &errorLine);
};
#endif