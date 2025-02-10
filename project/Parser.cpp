#include "Parser.hpp"
#include "Time.hpp"
#include "ParserHelpers.hpp"
#include <fstream>

namespace Yadro {

bool Parser::readLines(const std::string &filename, std::string &errorLine) {
    std::ifstream fileStream(filename);
    if (!fileStream) {
        errorLine = "Error: Cannot open file " + filename;
        return false;
    }
    std::string line;
    while (std::getline(fileStream, line)) {
        while (!line.empty() && (line.back() == '\r'))
            line.pop_back();
        m_lines.push_back(line);
    }
    return true;
}

Parser::Parser(const std::string &filename) {
    std::string err;
    if (!readLines(filename, err)) {
        m_lines.push_back(err);
    }
}

bool Parser::ExecuteLines   (ClubConfig &config, std::vector<EventData> &events, std::string & errorLine) {
    int lineNum = 0;
    if (m_lines.size() < 3) {
        errorLine = "Not enough configuration lines provided.\nMust be:\n<Number of Tables>\n<Opening Hours> <Closing Hours>\n<Hourly Cost>.";
        return false;
    }

    if (!parseTableCount(m_lines[lineNum++], config, errorLine))
        return false;
    if (!parseOperatingHours(m_lines[lineNum++], config, errorLine))
        return false;
    if (!parseHourlyCost(m_lines[lineNum++], config, errorLine))
        return false;

    for (; lineNum < static_cast<int>(m_lines.size()); lineNum++) {
        std::string line = m_lines[lineNum];
        if (line.empty())
            continue;
        EventData event;
        if (!parseEvent(line, event, config, errorLine))
            return false;
        events.push_back(event);
    }
    return true;
}

}