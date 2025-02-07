#ifndef PARSER_HPP
#define PARSER_HPP

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


class Parser {
private:
    std::vector<std::string> lines;

public:
    Parser(const std::string &filename);
};

#endif