#ifndef CLUB_HPP
#define CLUB_HPP

#include "Parser.hpp"
#include <vector>
#include <set>
#include <unordered_map>
#include <string>

struct Table {
    int number;              // id number of the Table   
    bool occupied;         
    std::string currentClient; 
    int startTime;          
    int totalOccupied;      // Total time occupied in minutes
    int revenue;  

    Table(int num) : number(num), occupied(false), startTime(0), totalOccupied(0), revenue(0) {}
};

class Club {
public:
    Club(int tablesnum, int openTime, int closeTime, int hourlyCost);
    // Process incoming event (from file).
    void processEvent(const EventData &event);
    // End of the day: freeing up remaining tables and generating event ID 11.
    void endOfDay();
    // Returns a list of strings to be printed to the console (in the correct order).
    const std::vector<std::string>& getOutput() const;
    // Generates and returns a final report on tables.
    std::vector<std::string> getReport() const;
    // Returns the opening time in the format "HH:MM".
    std::string getOpenTimeStr() const;
    // Returns the closing time in the format "HH:MM".
    std::string getCloseTimeStr() const;
private:
    int tablesCount;
    int openTime;
    int closeTime;
    int hourlyCost;
    std::vector<Table> tables;
    std::set<std::string> currentClients;
    std::unordered_map<std::string, int> seatedClients;
    std::set<std::string> WaitingQueue;
    std::vector<std::string> outputEvents;

    // Add event to outputEvents vector of strings
    void addOutputEvent(const std::string& event);
    // Generate error event string with time and error message
    void processErrorEvent(int time, const std::string& errorMsg);
    // Compute client revenue based on minutes
    int computeClientRevenue(int minutes) const;

    void freeTable(int tableIndex, int eventTime);


    void processEventID1(int time, const std::string& client);
    void processEventID2(int time, const std::string& client, const int tableNumber);
    void processEventID3(int time, const std::string& client);
    void processEventID4(int time, const std::string& client);
};

#endif
