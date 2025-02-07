#include "Parser.hpp"
#include "CheckConfigLines.hpp"

bool FileParser::readLines(const std::string &filename, std::string &errorLine) {
    FILE *file = std::fopen(filename.c_str(), "r");
    if (!file) {
        errorLine = "Error: Cannot open file " + filename;
        return false;
    }
    
    char buffer[4096];

    while (std::fgets(buffer, sizeof(buffer), file)) {
        std::string line(buffer);
        while (!line.empty() && (line.back() == '\n')) {
            line.pop_back();
        }
        lines.push_back(line);
    }
    
    std::fclose(file);
    return true;
}

FileParser::FileParser(const std::string &filename) {
    std::string err;
    if (!readLines(filename, err)) {
        lines.push_back(err);
    }
}

bool FileParser::Start(ClubConfig &config, std::vector<EventData> &events, std::string & errorLine) {
    int lineNum = 0;
    if (lines.size() < 3){
        errorLine = "Not enough configuration lines provided.\nMust be:\n<Number of Tables>\n<Opening Hours> <Closing Hours>\n<Hourly Cost>.";
        return false;
    }

    if (!checkTableCount(lines[lineNum++], config, errorLine))
        return false;
    if (!checkOperatingHours(lines[lineNum++], config, errorLine))
        return false;
    if (!checkHourlyCost(lines[lineNum++], config, errorLine))
        return false;

    for (; lineNum < static_cast<int>(lines.size()); lineNum++) {
        std::string line = lines[lineNum];
        if (line.empty())
            continue;
        EventData event;
        if (!checkEventLine(line, event, errorLine))
            return false;
        events.push_back(event);
    }
    
    return true;
}