#ifndef CLUB_HPP
#define CLUB_HPP

#include "Parser.hpp"

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
    Club(int tablesCount, int openTime, int closeTime, int hourlyCost);
private:
    int tablesCount;
    int openTime;
    int closeTime;
    int hourlyCost;
    std::vector<Table> tables;
};

#endif