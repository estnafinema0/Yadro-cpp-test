#include <gtest/gtest.h>
#include "../../project/Time.hpp"

TEST(ParseTimeTest, ValidTimes_LowestBoundary) {
    bool success = false;
    int minutes = Time::FromString("00:00", success);
    EXPECT_TRUE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, ValidTimes_HighestBoundary) {
    bool success = false;
    int minutes = Time::FromString("23:59", success);
    EXPECT_TRUE(success);
    EXPECT_EQ(minutes, 23 * 60 + 59);
}

TEST(ParseTimeTest, ValidTimes_MidRange) {
    bool success = false;
    int minutes = Time::FromString("12:34", success);
    EXPECT_TRUE(success);
    EXPECT_EQ(minutes, 12 * 60 + 34);
}

TEST(ParseTimeTest, InvalidFormat_NegativeHours1) {
    bool success = false;
    int minutes = Time::FromString("-23:00", success); 
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidFormat_NegativeHours2) {
    bool success = false;
    int minutes = Time::FromString("-23:00", success); 
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidFormat_LengthLessThan5) {
    bool success = false;
    int minutes = Time::FromString("9:00", success); 
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidFormat_LengthMoreThan5) {
    bool success = false;
    int minutes = Time::FromString("09:000", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidFormat_NoColonAtPosition2) {
    bool success = false;
    int minutes = Time::FromString("12-34", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidNumeric_HoursNotNumeric) {
    bool success = false;
    int minutes = Time::FromString("ab:15", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidNumeric_MinutesNotNumeric) {
    bool success = false;
    int minutes = Time::FromString("12:xx", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidRange_HoursTooHigh) {
    bool success = false;
    int minutes = Time::FromString("24:00", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidRange_HoursNegative) {
    bool success = false;
    int minutes = Time::FromString("-1:30", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidRange_MinutesTooHigh) {
    bool success = false;
    int minutes = Time::FromString("23:60", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, InvalidRange_MinutesNegative) {
    bool success = false;
    int minutes = Time::FromString("14:-1", success);
    EXPECT_FALSE(success);
    EXPECT_EQ(minutes, 0);
}

TEST(FormatTimeTest, FormatZeroMinutes) {
    std::string formatted = Time::ToString(0);
    EXPECT_EQ(formatted, "00:00");
}

TEST(FormatTimeTest, FormatNormalTime) {
    std::string formatted = Time::ToString(12 * 60 + 34);
    EXPECT_EQ(formatted, "12:34");
}

TEST(FormatTimeTest, FormatBoundary) {
    std::string formatted = Time::ToString(23 * 60 + 59);
    EXPECT_EQ(formatted, "23:59");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
