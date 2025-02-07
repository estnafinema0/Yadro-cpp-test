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
private:
    std::vector<std::string> lines;

public:
    FileParser(const std::string &filename);
    bool readLines(const std::string &filename, std::string &errorLine);
};

#endif