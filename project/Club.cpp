#include "Club.hpp"
#include "Time.hpp"
#include <sstream>
#include <algorithm>

namespace Yadro {

Club::Club(int tablesnum, int openTime, int closeTime, int hourlyCost)
    : m_tablesCount(tablesnum), m_openTime(openTime), m_closeTime(closeTime), m_hourlyCost(hourlyCost) {
    for (int i = 1; i <= m_tablesCount; i++)
        m_tables.push_back(Table(i));
}

std::string Club::getOpenTimeStr() const {
    return Time::ToString(m_openTime);
}

std::string Club::getCloseTimeStr() const {
    return Time::ToString(m_closeTime);
}

const std::vector<std::string>& Club::getOutput() const {
    return m_outputEvents;
}

void Club::addOutputEvent(const std::string &event) {
    m_outputEvents.push_back(event);
}

int Club::computeClientRevenue(int minutes) const {
    return ((minutes + 59) / 60) * m_hourlyCost;
}

void Club::processEvent(const EventData &event) {
    addOutputEvent(event.originalLine);
    if (event.eventId == 1)
        processEventID1(event.time, event.ClientName);
    else if (event.eventId == 2)
        processEventID2(event.time, event.ClientName, event.TableNumber);
    else if (event.eventId == 3)
        processEventID3(event.time, event.ClientName);
    else if (event.eventId == 4)
        processEventID4(event.time, event.ClientName);
}

void Club::processEventID1(int time, const std::string &client) {
    if (time < m_openTime || time > m_closeTime) {
        processErrorEvent(time, "NotOpenYet");
        return;
    }
    if (m_currentClients.find(client) != m_currentClients.end()) {
        processErrorEvent(time, "YouShallNotPass");
        return;
    }
    m_currentClients.insert(client);
}

void Club::processEventID2(int time, const std::string &client, int tableNumber) {
    if (m_currentClients.find(client) == m_currentClients.end()) {
        processErrorEvent(time, "ClientUnknown");
        return;
    }
    if (tableNumber < 1 || tableNumber > m_tablesCount) {
        processErrorEvent(time, "InvalidTableNumber");
        return;
    }
    int tableIndex = tableNumber - 1;
    if (m_tables[tableIndex].occupied) {
        processErrorEvent(time, "PlaceIsBusy");
        return;
    }
    if (m_seatedClients.find(client) != m_seatedClients.end()) {
        int oldTableNum = m_seatedClients[client];
        int oldTableIndex = oldTableNum - 1;
        freeTable(oldTableIndex, time);
    }
    m_tables[tableIndex].occupied = true;
    m_tables[tableIndex].currentClient = client;
    m_tables[tableIndex].startTime = time;
    m_seatedClients[client] = tableNumber;
}

void Club::processEventID3(int time, const std::string &client) {
    bool freeExists = false;
    for (const auto &table : m_tables) {
        if (!table.occupied) {
            freeExists = true;
            break;
        }
    }
    if (freeExists) {
        processErrorEvent(time, "ICanWaitNoLonger!");
        return;
    }
    if (std::find(m_waitingQueue.begin(), m_waitingQueue.end(), client) == m_waitingQueue.end())
        m_waitingQueue.push_back(client);
    if (static_cast<int>(m_waitingQueue.size()) > m_tablesCount) {
        m_waitingQueue.erase(std::remove(m_waitingQueue.begin(), m_waitingQueue.end(), client), m_waitingQueue.end());
        m_currentClients.erase(client);
        m_seatedClients.erase(client);
        std::ostringstream oss;
        oss << Time::ToString(time) << " 11 " << client;
        addOutputEvent(oss.str());
    }
}

void Club::processEventID4(int time, const std::string &client) {
    if (m_currentClients.find(client) == m_currentClients.end()) {
        processErrorEvent(time, "ClientUnknown");
        return;
    }
    if (m_seatedClients.find(client) != m_seatedClients.end()) {
        int tableNum = m_seatedClients[client];
        int tableIndex = tableNum - 1;
        freeTable(tableIndex, time);
    } else {
        m_waitingQueue.erase(std::remove(m_waitingQueue.begin(), m_waitingQueue.end(), client), m_waitingQueue.end());
    }
    m_currentClients.erase(client);
}

void Club::freeTable(int tableIndex, int eventTime) {
    if (m_tables[tableIndex].occupied) {
        int duration = eventTime - m_tables[tableIndex].startTime;
        if (duration < 0)
            duration = 0;
        m_tables[tableIndex].totalOccupied += duration;
        m_tables[tableIndex].revenue += computeClientRevenue(duration);
        std::string client = m_tables[tableIndex].currentClient;
        m_tables[tableIndex].occupied = false;
        m_tables[tableIndex].currentClient = "";
        m_seatedClients.erase(client);

        if (eventTime != m_closeTime) {
            assignTableToWaiting(tableIndex, eventTime);
        }
    }
}

void Club::assignTableToWaiting(int tableIndex, int eventTime) {
    if (m_waitingQueue.empty())
        return;
    std::string client = m_waitingQueue.front();
    m_waitingQueue.erase(m_waitingQueue.begin());
    m_tables[tableIndex].occupied = true;
    m_tables[tableIndex].currentClient = client;
    m_tables[tableIndex].startTime = eventTime;
    m_seatedClients[client] = m_tables[tableIndex].number;
    std::ostringstream oss;
    oss << Time::ToString(eventTime) << " 12 " << client << " " << m_tables[tableIndex].number;
    addOutputEvent(oss.str());
}

void Club::processErrorEvent(int time, const std::string &errorMsg) {
    std::ostringstream oss;
    oss << Time::ToString(time) << " 13 " << errorMsg;
    addOutputEvent(oss.str());
}

void Club::endOfDay() {
    for (auto &table : m_tables) {
        if (table.occupied)
            freeTable(table.number - 1, m_closeTime);
    }
    std::vector<std::string> remainingClients(m_currentClients.begin(), m_currentClients.end());
    std::sort(remainingClients.begin(), remainingClients.end());
    for (const auto &client : remainingClients) {
        std::ostringstream oss;
        oss << Time::ToString(m_closeTime) << " 11 " << client;
        addOutputEvent(oss.str());
    }
}

std::vector<std::string> Club::getReport() const {
    std::vector<std::string> report;
    for (const auto &table : m_tables) {
        std::string occTime = Time::ToString(table.totalOccupied);
        std::ostringstream oss;
        oss << table.number << " " << table.revenue << " " << occTime;
        report.push_back(oss.str());
    }
    return report;
}

}