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
    explicit FileParser(const std::string &filename);
    bool Start(ClubConfig &config, std::vector<EventData> &events, std::string & errorLine);
private:
    std::vector<std::string> lines;
    bool readLines(const std::string &filename, std::string &errorLine);
};
#endif