#include "Club.hpp"



Club::Club(int tablesCount, int openTime, int closeTime, int hourlyCost): tablesCount(tablesCount), openTime(openTime), closeTime(closeTime), hourlyCost(hourlyCost)
{
    for (int i = 1; i <= tablesCount; i++)
        tables.push_back(Table(i));
}

