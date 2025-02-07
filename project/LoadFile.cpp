#include "LoadFile.hpp"

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