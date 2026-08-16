#include <gtest/gtest.h>
#include "ondemand/parser/parse_number.hpp"

using namespace zuu::json;

TEST(ParseNumberTest, FastInteger) {
    // Exactly 8 digits (SWAR path)
    auto res1 = ondemand::parser::ParseDoubleFast("12345678");
    ASSERT_TRUE(res1.has_value());
    EXPECT_DOUBLE_EQ(res1.value(), 12345678.0);

    // Negative 8 digits
    auto res2 = ondemand::parser::ParseDoubleFast("-87654321");
    ASSERT_TRUE(res2.has_value());
    EXPECT_DOUBLE_EQ(res2.value(), -87654321.0);
    
    // More than 8 digits
    auto res3 = ondemand::parser::ParseDoubleFast("1234567890");
    ASSERT_TRUE(res3.has_value());
    EXPECT_DOUBLE_EQ(res3.value(), 1234567890.0);

    // Less than 8 digits
    auto res4 = ondemand::parser::ParseDoubleFast("123");
    ASSERT_TRUE(res4.has_value());
    EXPECT_DOUBLE_EQ(res4.value(), 123.0);
}

TEST(ParseNumberTest, FastFloat) {
    auto res1 = ondemand::parser::ParseDoubleFast("3.14159");
    ASSERT_TRUE(res1.has_value());
    EXPECT_DOUBLE_EQ(res1.value(), 3.14159);
    
    auto res2 = ondemand::parser::ParseDoubleFast("-0.001");
    ASSERT_TRUE(res2.has_value());
    EXPECT_DOUBLE_EQ(res2.value(), -0.001);
}

TEST(ParseNumberTest, Exponent) {
    auto res1 = ondemand::parser::ParseDoubleFast("1.5e3");
    ASSERT_TRUE(res1.has_value());
    EXPECT_DOUBLE_EQ(res1.value(), 1500.0);
    
    auto res2 = ondemand::parser::ParseDoubleFast("-2.5E-2");
    ASSERT_TRUE(res2.has_value());
    EXPECT_DOUBLE_EQ(res2.value(), -0.025);
    
    // Fallback to std::from_chars (very complex format or extra chars)
    // Actually our fast path handles this but let's make sure it's correct
    auto res3 = ondemand::parser::ParseDoubleFast("1.2345678e+5");
    ASSERT_TRUE(res3.has_value());
    EXPECT_DOUBLE_EQ(res3.value(), 123456.78);
}

TEST(ParseNumberTest, FallbackAndErrors) {
    // Garbage at end triggers fallback, which fails because ptr != str.end()
    auto res1 = ondemand::parser::ParseDoubleFast("123.45abc");
    EXPECT_FALSE(res1.has_value());
    EXPECT_EQ(res1.error(), Error::InvalidFormat);

    auto res2 = ondemand::parser::ParseDoubleFast("");
    EXPECT_FALSE(res2.has_value());
    EXPECT_EQ(res2.error(), Error::InvalidFormat);
}
