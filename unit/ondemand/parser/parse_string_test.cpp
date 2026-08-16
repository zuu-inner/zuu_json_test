#include <gtest/gtest.h>
#include "ondemand/parser/parse_string.hpp"

using namespace zuu::json;

TEST(ParseStringTest, ValidASCII) {
    std::string_view valid = "Hello World!";
    auto res = ondemand::parser::ValidateUtf8(valid);
    EXPECT_TRUE(res.has_value());
}

TEST(ParseStringTest, ValidUTF8) {
    // UTF-8 string: "Hello \xF0\x9F\x8C\x8D" (Hello 🌍)
    std::string_view valid = "Hello \xF0\x9F\x8C\x8D";
    auto res = ondemand::parser::ValidateUtf8(valid);
    EXPECT_TRUE(res.has_value());
    
    // Japanese "こんにちは"
    std::string_view jp = "\xE3\x81\x93\xE3\x82\x93\xE3\x81\xAB\xE3\x81\xA1\xE3\x81\xAF";
    auto res_jp = ondemand::parser::ValidateUtf8(jp);
    EXPECT_TRUE(res_jp.has_value());
}

TEST(ParseStringTest, InvalidUTF8) {
    // Invalid sequence (missing continuation bytes)
    std::string_view invalid_missing = "\xE3\x81";
    auto res1 = ondemand::parser::ValidateUtf8(invalid_missing);
    EXPECT_FALSE(res1.has_value());
    EXPECT_EQ(res1.error(), Error::InvalidUnicode);

    // Invalid start byte (0xFF)
    std::string_view invalid_start = "bad \xFF string";
    auto res2 = ondemand::parser::ValidateUtf8(invalid_start);
    EXPECT_FALSE(res2.has_value());
    EXPECT_EQ(res2.error(), Error::InvalidUnicode);

    // Overlong encoding (e.g. '/' encoded as 2 bytes)
    std::string_view invalid_overlong = "\xC0\xAF";
    auto res3 = ondemand::parser::ValidateUtf8(invalid_overlong);
    EXPECT_FALSE(res3.has_value());
    EXPECT_EQ(res3.error(), Error::InvalidUnicode);
}

TEST(ParseStringTest, UnescapeString) {
    // Normal string without escapes
    std::string_view normal = "Hello";
    auto res_normal = ondemand::parser::UnescapeString(normal);
    ASSERT_TRUE(res_normal.has_value());
    EXPECT_EQ(res_normal.value(), "Hello");

    // String with escapes
    std::string_view escaped = "Line1\\nLine2\\t\\\"Hello\\\"\\\\";
    auto res_escaped = ondemand::parser::UnescapeString(escaped);
    ASSERT_TRUE(res_escaped.has_value());
    EXPECT_EQ(res_escaped.value(), "Line1\nLine2\t\"Hello\"\\");

    // String with Unicode escapes (Earth globe: U+1F30D -> \uD83C\uDF0D)
    std::string_view unicode = "\\uD83C\\uDF0D";
    auto res_unicode = ondemand::parser::UnescapeString(unicode);
    ASSERT_TRUE(res_unicode.has_value());
    EXPECT_EQ(res_unicode.value(), "\xF0\x9F\x8C\x8D");

    // Invalid escape
    std::string_view invalid_esc = "bad\\x";
    auto res_invalid = ondemand::parser::UnescapeString(invalid_esc);
    EXPECT_FALSE(res_invalid.has_value());
    EXPECT_EQ(res_invalid.error(), Error::InvalidFormat);
}
