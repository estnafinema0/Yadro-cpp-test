#include <gtest/gtest.h>
#include "../../project/Parser.hpp"
#include <fstream>
#include <cstdio>
#include <string>

// Helper function to write content to a temporary file.
bool writeToFile(const std::string &filename, const std::string &content) {
    std::ofstream ofs(filename);
    if (!ofs)
        return false;
    ofs << content;
    return true;
}

// Use the same name for the temporary file; it is deleted after each test.
static const std::string tempFileName = "temp_parser_test.txt";

// Deletes the temporary file.
void removeTempFile() {
    std::remove(tempFileName.c_str());
}

// *************************
// Tests for configuration errors (first 3 lines)
// *************************

// 1. Less than 3 config lines
TEST(ParserErrorTest, InsufficientConfigLines) {
    std::string content = "3\n09:00 19:00";  
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "Not enough configuration lines provided.\nMust be:\n<Number of Tables>\n<Opening Hours> <Closing Hours>\n<Hourly Cost>.");
    removeTempFile();
}

// 2. First line: multiple tokens.
TEST(ParserErrorTest, Config_FirstLine_MultipleTokens) {
    std::string content = "3 4\n09:00 19:00\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "3 4");
    removeTempFile();
}

// 3. First line: non-integer value.
TEST(ParserErrorTest, Config_FirstLine_NonInteger) {
    std::string content = "abc\n09:00 19:00\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "abc");
    removeTempFile();
}

// 4. First line: non-positive number.
TEST(ParserErrorTest, Config_FirstLine_NonPositive) {
    std::string content = "0\n09:00 19:00\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "0");
    removeTempFile();
}

// 5. Second line: less than 2 tokens.
TEST(ParserErrorTest, Config_SecondLine_OneToken) {
    std::string content = "3\n09:00\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00");
    removeTempFile();
}

// 6. Second line: more than 2 tokens.
TEST(ParserErrorTest, Config_SecondLine_ThreeTokens) {
    std::string content = "3\n09:00 19:00 extra\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 19:00 extra");
    removeTempFile();
}

// 7. Second line: invalid time format (e.g. "9:00" instead of "09:00").
TEST(ParserErrorTest, Config_SecondLine_InvalidTime_Open) {
    std::string content = "3\n9:00 19:00\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "9:00 19:00");
    removeTempFile();
}

// 8. Second line: invalid time format (e.g. 19:60).
TEST(ParserErrorTest, Config_SecondLine_InvalidTime_Close) {
    std::string content = "3\n09:00 19:60\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 19:60");
    removeTempFile();
}

// 9. Second line: opening time is not before closing time (e.g. 19:00 09:00).
TEST(ParserErrorTest, Config_SecondLine_OpenNotBeforeClose) {
    std::string content = "3\n19:00 09:00\n10";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "19:00 09:00");
    removeTempFile();
}

// 10. Third line: multiple tokens.
TEST(ParserErrorTest, Config_ThirdLine_MultipleTokens) {
    std::string content = "3\n09:00 19:00\n10 20";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "10 20");
    removeTempFile();
}

// 11. Third line: non-integer value.
TEST(ParserErrorTest, Config_ThirdLine_NonInteger) {
    std::string content = "3\n09:00 19:00\nabc";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "abc");
    removeTempFile();
}

// 12. Third line: non-positive number.
TEST(ParserErrorTest, Config_ThirdLine_NonPositive) {
    std::string content = "3\n09:00 19:00\n0";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "0");
    removeTempFile();
}

// *************************
// Tests for event lines (from line 4)
// *************************

// For all event tests, use valid first 3 lines.
const std::string validConfig = "3\n09:00 19:00\n10\n";

// 13. Event: less than 2 tokens.
TEST(ParserErrorTest, EventLine_LessThanTwoTokens) {
    std::string content = validConfig + "09:00";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00");
    removeTempFile();
}

// 14. Event: invalid time format (e.g. "9:00" instead of "09:00").
TEST(ParserErrorTest, EventLine_InvalidTime) {
    std::string content = validConfig + "9:00 1 client";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "9:00 1 client");
    removeTempFile();
}

// 15. Event: invalid event ID (non-integer).
TEST(ParserErrorTest, EventLine_InvalidEventId_NonInteger) {
    std::string content = validConfig + "09:00 abc client";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 abc client");
    removeTempFile();
}

// 16. Event: invalid event ID (0).
TEST(ParserErrorTest, EventLine_InvalidEventId_OutOfRange_Low) {
    std::string content = validConfig + "09:00 0 client";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 0 client");
    removeTempFile();
}

// 17. Event: invalid event ID (5).
TEST(ParserErrorTest, EventLine_InvalidEventId_OutOfRange_High) {
    std::string content = validConfig + "09:00 5 client";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events   ;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 5 client");
    removeTempFile();
}

// 18. Event: wrong token count for event ID 1 (too few).
TEST(ParserErrorTest, EventLine_WrongTokenCount_EventType1_TooFew) {
    std::string content = validConfig + "09:00 1";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 1");
    removeTempFile();
}

// 19. Event: wrong token count for event ID 1 (too many).
TEST(ParserErrorTest, EventLine_WrongTokenCount_EventType1_TooMany) {
    std::string content = validConfig + "09:00 1 client extra";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 1 client extra");
    removeTempFile();
}

// 20. Event: wrong token count for event ID 2 (too few).
TEST(ParserErrorTest, EventLine_WrongTokenCount_EventType2_TooFew) {
    std::string content = validConfig + "09:00 2 client";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 2 client");
    removeTempFile();
}

// 21. Event: wrong token count for event ID 2 (too many).
TEST(ParserErrorTest, EventLine_WrongTokenCount_EventType2_TooMany) {
    std::string content = validConfig + "09:00 2 client 1 extra";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_FALSE(parser.Start(config, events, errorLine));
    EXPECT_EQ(errorLine, "09:00 2 client 1 extra");
    removeTempFile();
}

// *************************
// Test for valid file content
// *************************

TEST(ParserErrorTest, ValidFile) {
    std::string content =
        "3\n"
        "09:00 19:00\n"
        "10\n"
        "08:48 1 client1\n"
        "09:41 1 client2\n"
        "10:00 2 client1 1\n"
        "10:30 3 client2\n"
        "11:00 4 client1\n";
    ASSERT_TRUE(writeToFile(tempFileName, content));

    FileParser parser(tempFileName);
    ClubConfig config;
    std::vector<EventData> events;
    std::string errorLine;
    EXPECT_TRUE(parser.Start(config, events, errorLine));
    // Check if config is parsed correctly.
    EXPECT_EQ(config.numTables, 3);
    EXPECT_EQ(config.openTime, 9 * 60);
    EXPECT_EQ(config.closeTime, 19 * 60);
    EXPECT_EQ(config.hourlyCost, 10);
    // There should be 5 events.
    EXPECT_EQ(events.size(), 5);
    removeTempFile();
}
