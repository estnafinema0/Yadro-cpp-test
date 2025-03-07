#include "Club.hpp"
#include "Parser.hpp"
#include <iostream>
#include <vector>
#include <string>


int main(int argc, char* argv[]) {
    using namespace Yadro;
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    Parser parser(filename);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    if (!parser.ExecuteLines(config, events, errorLine)) {
        // Output the first line with the error and terminate the program.
        std::cout << errorLine << std::endl;
        return 0;
    }

    Club club(config.numTables, config.openTime, config.closeTime, config.hourlyCost);

    std::vector<std::string> output;
    output.push_back(club.getOpenTimeStr());
    for (const auto &event : events) {
        club.processEvent(event);
    }

    club.endOfDay();
    // Now we have all the events processed and the output is ready.
    const std::vector<std::string> &eventOutputs = club.getOutput();
    for (const auto &line : eventOutputs) {
        output.push_back(line);
    }
    output.push_back(club.getCloseTimeStr());

    // Making the final report about the revenue.
    std::vector<std::string> report = club.getReport();
    for (const auto &line : report) {
        output.push_back(line);
    }

    for (const auto &line : output) {
        std::cout << line << std::endl;
    }
    
    return 0;
}