#ifndef CLUB_HPP
#define CLUB_HPP

#include "LoadFile.hpp"

struct Table {
    int number;                 
    bool occupied;         
    std::string currentClient; 
    int startTime;          
    int totalOccupied;      // Total time occupied
    int revenue;          
    Table(int num) : number(num), occupied(false), startTime(0), totalOccupied(0), revenue(0) {}
};

#endif