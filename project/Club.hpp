#pragma once

#include "Parser.hpp"
#include <vector>
#include <set>
#include <unordered_map>
#include <string>

namespace Yadro {

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
    void processEvent(const EventData &event);
    void endOfDay();
    const std::vector<std::string>& getOutput() const;
    std::vector<std::string> getReport() const;
    std::string getOpenTimeStr() const;
    std::string getCloseTimeStr() const;
private:
    int m_tablesCount;
    int m_openTime;
    int m_closeTime;
    int m_hourlyCost;
    std::vector<Table> m_tables;
    std::set<std::string> m_currentClients;
    std::unordered_map<std::string, int> m_seatedClients;
    std::vector<std::string> m_waitingQueue;
    std::vector<std::string> m_outputEvents;

    void addOutputEvent(const std::string &event);
    void processErrorEvent(int time, const std::string &errorMsg);
    int computeClientRevenue(int minutes) const;
    void freeTable(int tableIndex, int eventTime);
    void assignTableToWaiting(int tableIndex, int eventTime);

    void processEventID1(int time, const std::string &client);
    void processEventID2(int time, const std::string &client, int tableNumber);
    void processEventID3(int time, const std::string &client);
    void processEventID4(int time, const std::string &client);
};

}