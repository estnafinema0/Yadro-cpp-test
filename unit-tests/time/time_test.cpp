#include <gtest/gtest.h>
#include "../../project/Time.hpp"
#include <optional>

using Yadro::Time::FromString;
using Yadro::Time::ToString;

TEST(ParseTimeTest, ValidTimes_LowestBoundary) {
    auto maybeMinutes = FromString("00:00");
    EXPECT_TRUE(maybeMinutes.has_value());
    int minutes = maybeMinutes.value();
    EXPECT_EQ(minutes, 0);
}

TEST(ParseTimeTest, ValidTimes_HighestBoundary) {
    auto maybeMinutes = FromString("23:59");
    EXPECT_TRUE(maybeMinutes.has_value());
    int minutes = maybeMinutes.value();
    EXPECT_EQ(minutes, 23 * 60 + 59);
}

TEST(ParseTimeTest, ValidTimes_MidRange) {
    auto maybeMinutes = FromString("12:34");
    EXPECT_TRUE(maybeMinutes.has_value());
    int minutes = maybeMinutes.value();
    EXPECT_EQ(minutes, 12 * 60 + 34);
}

TEST(ParseTimeTest, InvalidFormat_NegativeHours1) {
    auto maybeMinutes = FromString("-23:00");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidFormat_NegativeHours2) {
    auto maybeMinutes = FromString("-23:00");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidFormat_LengthLessThan5) {
    auto maybeMinutes = FromString("9:00");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidFormat_LengthMoreThan5) {
    auto maybeMinutes = FromString("09:000");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidFormat_NoColonAtPosition2) {
    auto maybeMinutes = FromString("12-34");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidNumeric_HoursNotNumeric) {
    auto maybeMinutes = FromString("ab:15");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidNumeric_MinutesNotNumeric) {
    auto maybeMinutes = FromString("12:xx");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidRange_HoursTooHigh) {
    auto maybeMinutes = FromString("24:00");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidRange_HoursNegative) {
    auto maybeMinutes = FromString("-1:30");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidRange_MinutesTooHigh) {
    auto maybeMinutes = FromString("23:60");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(ParseTimeTest, InvalidRange_MinutesNegative) {
    auto maybeMinutes = FromString("14:-1");
    EXPECT_FALSE(maybeMinutes.has_value());
}

TEST(FormatTimeTest, FormatZeroMinutes) {
    std::string formatted = ToString(0);
    EXPECT_EQ(formatted, "00:00");
}

TEST(FormatTimeTest, FormatNormalTime) {
    std::string formatted = ToString(12 * 60 + 34);
    EXPECT_EQ(formatted, "12:34");
}

TEST(FormatTimeTest, FormatBoundary) {
    std::string formatted = ToString(23 * 60 + 59);
    EXPECT_EQ(formatted, "23:59");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
