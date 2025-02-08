#include "Club.hpp"
#include "Time.hpp"


Club::Club(int tablesCount, int openTime, int closeTime, int hourlyCost): tablesCount(tablesCount), openTime(openTime), closeTime(closeTime), hourlyCost(hourlyCost)
{
    for (int i = 1; i <= tablesCount; i++)
        tables.push_back(Table(i));
}

std::string Club::getOpenTimeStr() const{
    return Time::ToString(openTime);
}

std::string Club::getCloseTimeStr() const{
    return Time::ToString(closeTime);
}

