#include "Club.hpp"
#include "Parser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    FileParser parser(filename);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    if (!parser.Start(config, events, errorLine)) {
        // Output the first line with the error and terminate the program.
        std::cout << errorLine << std::endl;
        return 0;
    }

    return 0;
}