#include <gtest/gtest.h>
#include "../../project/Club.hpp"
#include "../../project/Time.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <optional>

using Yadro::Club;
using Yadro::EventData;
using Yadro::Time::FromString; 
using Yadro::Time::ToString;

// Helper function to create an event (EventData)
EventData createEvent(const std::string &timeStr, int id, const std::string &client, const int tableNumber = -1) {
    EventData event;
    auto maybeTime = FromString(timeStr);
    ASSERT_TRUE(maybeTime.has_value());  
    event.time = maybeTime.value();
    event.eventId = id;
    event.ClientName = client;
    event.TableNumber = tableNumber;
    std::string line = timeStr + " " + std::to_string(id) + " " + client;
    if (tableNumber != -1) {
        line += " " + std::to_string(tableNumber);
    }
    event.originalLine = line;
    return event;
}

class ClubTest : public ::testing::Test {
protected:
    int numTables = 3;
    int openTime = 9 * 60;   
    int closeTime = 19 * 60; 
    int hourlyCost = 10;
    Club *club;
    
    virtual void SetUp() {
        club = new Club(numTables, openTime, closeTime, hourlyCost);
    }
    virtual void TearDown() {
        delete club;
    }
};


// =========================
// Tests for event type ID 1 (Client arrives)
// =========================

// 1. Client arrives before opening - error "NotOpenYet"
TEST_F(ClubTest, ClientArrival_NotOpenYet) {
    // Time 08:50 (<09:00)
    EventData event = createEvent("08:50", 1, "client1");
    club->processEvent(event);
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 2);
    EXPECT_EQ(outputs[0], "08:50 1 client1");
    EXPECT_EQ(outputs[1], "08:50 13 NotOpenYet");
}

// 2. Client tries to arrive twice - error "YouShallNotPass"
TEST_F(ClubTest, ClientArrival_AlreadyInClub) {
    EventData event1 = createEvent("09:10", 1, "client1");
    club->processEvent(event1);

    EventData event2 = createEvent("09:20", 1, "client1");
    club->processEvent(event2);
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 3);
    EXPECT_EQ(outputs[0], "09:10 1 client1");
    EXPECT_EQ(outputs[1], "09:20 1 client1");
    EXPECT_EQ(outputs[2], "09:20 13 YouShallNotPass");
}

// =========================
// Tests for event type ID 2 (Client sits at a table)
// =========================

// 3. Client, who is not in the club, tries to sit - error "ClientUnknown"
TEST_F(ClubTest, ClientSit_ClientUnknown) {
    EventData event = createEvent("10:00", 2, "client1", 1);
    club->processEvent(event);
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 2);
    EXPECT_EQ(outputs[0], "10:00 2 client1 1");
    EXPECT_EQ(outputs[1], "10:00 13 ClientUnknown");
}

// 5. Attempt to sit at a busy table - error "PlaceIsBusy"
TEST_F(ClubTest, ClientSit_PlaceIsBusy) {
    // client1 sits at table 1
    club->processEvent(createEvent("10:00", 1, "client1"));
    club->processEvent(createEvent("10:05", 2, "client1", 1));
    // client2 arrives and tries to sit at the same table
    club->processEvent(createEvent("10:10", 1, "client2"));
    club->processEvent(createEvent("10:15", 2, "client2", 1));
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 5);
    EXPECT_EQ(outputs[0], "10:00 1 client1");
    EXPECT_EQ(outputs[1], "10:05 2 client1 1");
    EXPECT_EQ(outputs[2], "10:10 1 client2");
    EXPECT_EQ(outputs[3], "10:15 2 client2 1");
    EXPECT_EQ(outputs[4], "10:15 13 PlaceIsBusy");
}

// 6. Client changes table (with release of the previous one) - normal scenario
TEST_F(ClubTest, ClientSit_ReassignTable) {
    // client1 arrives and sits at table 1
    club->processEvent(createEvent("10:00", 1, "client1"));
    club->processEvent(createEvent("10:05", 2, "client1", 1));
    // Then client1 changes table to 2
    club->processEvent(createEvent("11:00", 2, "client1", 2));

    club->endOfDay();
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 4);
    EXPECT_EQ(outputs[0], "10:00 1 client1");
    EXPECT_EQ(outputs[1], "10:05 2 client1 1");
    EXPECT_EQ(outputs[2], "11:00 2 client1 2");
    EXPECT_EQ(outputs[3], "19:00 11 client1");
    
    std::vector<std::string> report = club->getReport();
    ASSERT_EQ(report.size(), 3);
    EXPECT_EQ(report[0], "1 10 00:55");
    EXPECT_EQ(report[1], "2 80 08:00");
    EXPECT_EQ(report[2], "3 0 00:00");
}

// =========================
// Tests for event type ID 3 (Client waits)
// =========================

// 8. Client tries to wait, but there is a free table – error "ICanWaitNoLonger!"
TEST_F(ClubTest, ClientWait_ICanWaitNoLonger) {
    // client1 arrives, but does not sit – there is a free table
    club->processEvent(createEvent("10:00", 1, "client1"));
    club->processEvent(createEvent("10:10", 3, "client1"));
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 3);
    EXPECT_EQ(outputs[0], "10:00 1 client1");
    EXPECT_EQ(outputs[1], "10:10 3 client1");
    EXPECT_EQ(outputs[2], "10:10 13 ICanWaitNoLonger!");
}

// 9. Waiting queue overflow – client leaves immediately, generates event ID 11
TEST_F(ClubTest, ClientWait_WaitingQueueOverflow) {
    // Use club with 2 tables
    delete club;
    club = new Yadro::Club(2, openTime, closeTime, hourlyCost);
    
    club->processEvent(createEvent("10:00", 1, "client1"));
    club->processEvent(createEvent("10:01", 2, "client1", 1));
    club->processEvent(createEvent("10:00", 1, "client2"));
    club->processEvent(createEvent("10:01", 2, "client2", 2));
    
    club->processEvent(createEvent("10:05", 3, "client3")); // waitingQueue: [client3]
    club->processEvent(createEvent("10:07", 3, "client4")); // waitingQueue: [client3, client4]
    // Next client exceeds the limit (waiting queue becomes 3 > numTables(2))
    club->processEvent(createEvent("10:09", 3, "client5"));
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 8);
    EXPECT_EQ(outputs[4], "10:05 3 client3");
    EXPECT_EQ(outputs[5], "10:07 3 client4");
    EXPECT_EQ(outputs[6], "10:09 3 client5");
    EXPECT_EQ(outputs[7], "10:09 11 client5");
}

// =========================
// Tests for event type ID 4 (Client leaves)
// =========================

// 10. Client, who is not in the club, tries to leave – error "ClientUnknown"
TEST_F(ClubTest, ClientLeave_ClientUnknown) {
    EventData leaveEvent = createEvent("10:20", 4, "client1");
    club->processEvent(leaveEvent);
    
    const auto &outputs = club->getOutput();
    ASSERT_EQ(outputs.size(), 2);
    EXPECT_EQ(outputs[0], "10:20 4 client1");
    EXPECT_EQ(outputs[1], "10:20 13 ClientUnknown");
}

// 11. Client, seated at a table, leaves – the table is freed, and if there is a waiting queue, the first waiting client occupies it
TEST_F(ClubTest, ClientLeave_SeatedWithWaitingAssignment) {
    // Use club with 2 tables
    delete club;
    club = new Yadro::Club(2, openTime, closeTime, hourlyCost);
    
    // client1 and client2 occupy tables
    club->processEvent(createEvent("10:00", 1, "client1"));
    club->processEvent(createEvent("10:01", 2, "client1", 1));
    club->processEvent(createEvent("10:00", 1, "client2"));
    club->processEvent(createEvent("10:01", 2, "client2", 2));
    // client3 arrives and waits
    club->processEvent(createEvent("10:05", 1, "client3"));
    club->processEvent(createEvent("10:06", 3, "client3"));
    // client1 leaves
    club->processEvent(createEvent("10:10", 4, "client1"));
    
    const auto &outputs = club->getOutput();
    bool foundAssignment = false;
    for (const auto &line : outputs) {
        if (line == "10:10 12 client3 1") {
            foundAssignment = true;
            break;
        }
    }
    EXPECT_TRUE(foundAssignment);
    
    // End of the day: remaining clients (client2 and client3) should receive events ID 11 in alphabetical order.
    club->endOfDay();
    const auto &finalOutputs = club->getOutput();
    bool foundClient2 = false, foundClient3 = false;
    for (const auto &line : finalOutputs) {
        if (line == "19:00 11 client2") foundClient2 = true;
        if (line == "19:00 11 client3") foundClient3 = true;
    }
    EXPECT_TRUE(foundClient2);
    EXPECT_TRUE(foundClient3);
}

// =========================
// Test for endOfDay 
// =========================

// 12. End of the day: freeing occupied tables and generating event ID 11 for remaining clients
TEST_F(ClubTest, EndOfDay_ProcessRemainingClients) {
    // client1 arrives and sits at table 1, client2 – at table 2, client3 – waits
    club->processEvent(createEvent("09:30", 1, "client1"));
    club->processEvent(createEvent("09:35", 2, "client1", 1));
    club->processEvent(createEvent("09:40", 1, "client2"));
    club->processEvent(createEvent("09:45", 2, "client2", 2));
    club->processEvent(createEvent("09:50", 1, "client3"));
    club->processEvent(createEvent("09:55", 3, "client3"));
    
    club->endOfDay();
    
    const auto &outputs = club->getOutput();
    
    // Ensure that at end-of-day no assignment (ID 12) events occur.
    for (const auto &line : outputs) {
        if (line.substr(0, 5) == "19:00" && line.find(" 12 ") != std::string::npos) {
            FAIL() << "Unexpected assignment event (ID 12) at end-of-day: " << line;
        }
    }
    
    // Collect events generated at end-of-day (those with time "19:00" and event ID 11)
    std::vector<std::string> endEvents;
    for (const auto &line : outputs) {
        if (line.find(" 11 ") != std::string::npos && line.substr(0, 5) == "19:00") {
            endEvents.push_back(line);
        }
    }
    // Expect that remaining clients (client1, client2, client3) receive events ID 11 in alphabetical order
    std::vector<std::string> expectedOrder = {"19:00 11 client1", "19:00 11 client2", "19:00 11 client3"};
    EXPECT_EQ(endEvents, expectedOrder);
    
    // Check the report: tables where clients sat should have correctly calculated revenue and occupancy time.
    std::vector<std::string> report = club->getReport();
    ASSERT_EQ(report.size(), 3);
    EXPECT_EQ(report[0], "1 100 09:25");
    EXPECT_EQ(report[1], "2 100 09:15");
    EXPECT_EQ(report[2], "3 0 00:00");
}
