#include "Club.hpp"
#include "Time.hpp"
#include <sstream>
#include <algorithm>


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
    std::string clientName = client;
        if (currentClients.find(clientName) == currentClients.end()) {
            processErrorEvent(time, "ClientUnknown");
            return;
        }

        bool freeExists = false;
        for (auto &table : tables) {
            if (!table.occupied) {
                freeExists = true;
                break;
            }
        }
        if (freeExists) {
            processErrorEvent(time, "ICanWaitNoLonger!");
            return;
        }
        // If client is not in waiting queue, add him.
        if (std::find(waitingQueue.begin(), waitingQueue.end(), client) == waitingQueue.end())
            waitingQueue.push_back(client);
        // If number of waiting clients is greater than number of tables, client leaves immediately.
        if (static_cast<int>(waitingQueue.size()) > tablesCount) {
            // remove-erase idiom
            waitingQueue.erase(std::remove(waitingQueue.begin(), waitingQueue.end(), client), waitingQueue.end());
            currentClients.erase(client);
            seatedClients.erase(client);
            std::ostringstream oss;
            oss << Time::ToString(time) << " 11 " << client;
            addOutputEvent(oss.str());
        }
}

void Club::processEventID4(int time, const std::string& client){
    std::string clientName = client;
    if (currentClients.find(clientName) == currentClients.end()) {
        processErrorEvent(time, "ClientUnknown");
        return;
    }
    if (seatedClients.find(clientName) != seatedClients.end()) {
        int tableNum = seatedClients[clientName];
        int tableIndex = tableNum - 1;
        freeTable(tableIndex, time);
    }    
    else {
        waitingQueue.erase(std::remove(waitingQueue.begin(), waitingQueue.end(), clientName), waitingQueue.end());
    }
    currentClients.erase(clientName);
}

void Club::freeTable(int tableIndex, int eventTime){
    if (tables[tableIndex].occupied) {
        int duration = eventTime - tables[tableIndex].startTime;
        if (duration < 0) duration = 0;
        tables[tableIndex].totalOccupied += duration;
        int hours = computeClientRevenue(duration);
        tables[tableIndex].revenue += hours * hourlyCost;
        std::string client = tables[tableIndex].currentClient;
        tables[tableIndex].occupied = false;
        tables[tableIndex].currentClient = "";
        seatedClients.erase(client);    

        assignTableToWaiting(tableIndex, eventTime);
    }
}

const std::vector<std::string>& Club::getOutput() const {
    return outputEvents;
}

void Club::assignTableToWaiting(int tableIndex, int eventTime) {
    if (waitingQueue.empty())
        return;
    std::string client = waitingQueue.front();
    waitingQueue.erase(waitingQueue.begin());

    tables[tableIndex].occupied = true;
    tables[tableIndex].currentClient = client;
    tables[tableIndex].startTime = eventTime;
    seatedClients[client] = tables[tableIndex].number;
    // Generate event ID 12.
    std::ostringstream oss;
    oss << Time::ToString(eventTime) << " 12 " << client << " " << tables[tableIndex].number;
    addOutputEvent(oss.str());
}

void Club::processErrorEvent(int time, const std::string &errorMsg) {
   std::ostringstream oss;
   oss << Time::ToString(time) << " 13 " << errorMsg;
   addOutputEvent(oss.str());
}
