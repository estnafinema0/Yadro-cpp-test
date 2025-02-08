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

void Club::addOutputEvent(const std::string& event){
    outputEvents.push_back(event);
}

int Club::computeClientRevenue(int minutes) const{
    return (minutes + 59) / 60 * hourlyCost;
}

void Club::processEvent(const EventData &event){
    if (event.eventId == 1){
        processEventID1(event.time, event.ClientName);
    }
    else if (event.eventId == 2){
        processEventID2(event.time, event.ClientName, event.TableNumber);
    }
    else if (event.eventId == 3){
        processEventID3(event.time, event.ClientName);
    }
    else if (event.eventId == 4){
        processEventID4(event.time, event.ClientName);
    }
}

void Club::processEventID1(int time, const std::string& client){
    std::string clientName = client;
    if (time < openTime || time > closeTime){
        processErrorEvent(time, "Client arrived outside the opening hours");
        return;
    }
    if (currentClients.find(clientName) != currentClients.end()) {
            processErrorEvent(time, "YouShallNotPass");
            return;
        }
    currentClients.insert(clientName);
}

void Club::processEventID2(int time, const std::string& client, int tableNumber){
    std::string clientName = client;

     if (currentClients.find(clientName) == currentClients.end()) {
            processErrorEvent(time, "ClientUnknown");
            return;
        }
        if (tableNumber < 1 || tableNumber > tablesCount) {
            processErrorEvent(time, "InvalidTableNumber");
            return;
        }
        int tableIndex = tableNumber - 1;
        if (tables[tableIndex].occupied) {
            processErrorEvent(time, "PlaceIsBusy");
            return;
        }
        // If client is already seated at another table, free it.
        if (seatedClients.find(client) != seatedClients.end()) {
            int oldTableNum = seatedClients[client];
            int oldTableIndex = oldTableNum - 1;
            freeTable(oldTableIndex, time);
        }
        // Assign client to selected table
        tables[tableIndex].occupied = true;
        tables[tableIndex].currentClient = client;
        tables[tableIndex].startTime = time;
        seatedClients[client] = tableNumber;
}

void Club::processEventID3(int time, const std::string& client){

}

void Club::processEventID4(int time, const std::string& client){

}