#include <gtest/gtest.h>
#include "ondemand/parser/scanner.hpp"

using namespace zuu::json;

TEST(ScannerTest, EmptyObject) {
    std::string_view json = "{}";
    auto result = ondemand::parser::Scanner(json);
    ASSERT_TRUE(result.has_value());
    auto indices = result.value();
    ASSERT_EQ(indices.size(), 2);
    EXPECT_EQ(indices[0], 0); // {
    EXPECT_EQ(indices[1], 1); // }
}

TEST(ScannerTest, EmptyArray) {
    std::string_view json = "[ ]";
    auto result = ondemand::parser::Scanner(json);
    ASSERT_TRUE(result.has_value());
    auto indices = result.value();
    ASSERT_EQ(indices.size(), 2);
    EXPECT_EQ(indices[0], 0); // [
    EXPECT_EQ(indices[1], 2); // ]
}

TEST(ScannerTest, StructuralCharacters) {
    std::string_view json = "{ [ ] : , }";
    auto result = ondemand::parser::Scanner(json);
    ASSERT_TRUE(result.has_value());
    auto indices = result.value();
    ASSERT_EQ(indices.size(), 6);
    EXPECT_EQ(indices[0], 0); // {
    EXPECT_EQ(indices[1], 2); // [
    EXPECT_EQ(indices[2], 4); // ]
    EXPECT_EQ(indices[3], 6); // :
    EXPECT_EQ(indices[4], 8); // ,
    EXPECT_EQ(indices[5], 10); // }
}

TEST(ScannerTest, IgnoreInsideStrings) {
    std::string_view json = "{\":\": \"[,]\"}";
    auto result = ondemand::parser::Scanner(json);
    ASSERT_TRUE(result.has_value());
    auto indices = result.value();
    ASSERT_EQ(indices.size(), 7);
    EXPECT_EQ(indices[0], 0); // {
    EXPECT_EQ(indices[1], 1); // "
    EXPECT_EQ(indices[2], 3); // "
    EXPECT_EQ(indices[3], 4); // :
    EXPECT_EQ(indices[4], 6); // "
    EXPECT_EQ(indices[5], 10); // "
    EXPECT_EQ(indices[6], 11); // }
}

TEST(ScannerTest, EmptyStringError) {
    std::string_view json = "";
    auto result = ondemand::parser::Scanner(json);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), Error::EmptyValue);
}
