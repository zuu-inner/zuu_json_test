#include <gtest/gtest.h>
#include "ondemand/parser/iterator.hpp"
#include "ondemand/parser/scanner.hpp"

using namespace zuu::json;

TEST(IteratorTest, TypeIdentification) {
    std::string_view json = R"({"str": "value", "num": 123, "arr": [], "obj": {}, "bool": true, "null": null})";
    auto scanner_res = ondemand::parser::Scanner(json);
    ASSERT_TRUE(scanner_res.has_value());
    
    auto indices = scanner_res.value();
    ondemand::parser::Iterator iter(indices, json);

    // Initial type should be Object
    auto t = iter.type();
    ASSERT_TRUE(t.has_value());
    EXPECT_EQ(t.value(), constants::JsonType::Object);
    
    // Find string value
    auto iter_str = ondemand::parser::Iterator(iter).findKey("str");
    ASSERT_TRUE(iter_str.has_value());
    EXPECT_EQ(iter_str.value().type().value(), constants::JsonType::String);

    // Find number value
    auto iter_num = ondemand::parser::Iterator(iter).findKey("num");
    ASSERT_TRUE(iter_num.has_value());
    EXPECT_EQ(iter_num.value().type().value(), constants::JsonType::Float);

    // Find array value
    auto iter_arr = ondemand::parser::Iterator(iter).findKey("arr");
    ASSERT_TRUE(iter_arr.has_value());
    EXPECT_EQ(iter_arr.value().type().value(), constants::JsonType::Array);

    // Find object value
    auto iter_obj = ondemand::parser::Iterator(iter).findKey("obj");
    ASSERT_TRUE(iter_obj.has_value());
    EXPECT_EQ(iter_obj.value().type().value(), constants::JsonType::Object);

    // Find boolean value
    auto iter_bool = ondemand::parser::Iterator(iter).findKey("bool");
    ASSERT_TRUE(iter_bool.has_value());
    EXPECT_EQ(iter_bool.value().type().value(), constants::JsonType::Bool);

    // Find null value
    auto iter_null = ondemand::parser::Iterator(iter).findKey("null");
    ASSERT_TRUE(iter_null.has_value());
    EXPECT_EQ(iter_null.value().type().value(), constants::JsonType::Null);
}

TEST(IteratorTest, GetString) {
    std::string_view json = R"({"name": "Zuu", "city": "Jakarta"})";
    auto scanner_res = ondemand::parser::Scanner(json);
    ASSERT_TRUE(scanner_res.has_value());
    
    auto indices = scanner_res.value();
    ondemand::parser::Iterator iter(indices, json);

    auto name_iter = ondemand::parser::Iterator(iter).findKey("name");
    ASSERT_TRUE(name_iter.has_value());
    auto name_val = name_iter.value().getString();
    ASSERT_TRUE(name_val.has_value());
    EXPECT_EQ(name_val.value(), "Zuu");

    auto city_iter = ondemand::parser::Iterator(iter).findKey("city");
    ASSERT_TRUE(city_iter.has_value());
    auto city_val = city_iter.value().getString();
    ASSERT_TRUE(city_val.has_value());
    EXPECT_EQ(city_val.value(), "Jakarta");
}

TEST(IteratorTest, GetNumber) {
    std::string_view json = R"({"integer": 42, "float": 3.1415, "negative": -10})";
    auto scanner_res = ondemand::parser::Scanner(json);
    ASSERT_TRUE(scanner_res.has_value());
    
    auto indices = scanner_res.value();
    ondemand::parser::Iterator iter(indices, json);

    auto int_iter = ondemand::parser::Iterator(iter).findKey("integer");
    ASSERT_TRUE(int_iter.has_value());
    auto int_val = int_iter.value().getNumber();
    ASSERT_TRUE(int_val.has_value());
    EXPECT_DOUBLE_EQ(int_val.value(), 42.0);

    auto float_iter = ondemand::parser::Iterator(iter).findKey("float");
    ASSERT_TRUE(float_iter.has_value());
    auto float_val = float_iter.value().getNumber();
    ASSERT_TRUE(float_val.has_value());
    EXPECT_DOUBLE_EQ(float_val.value(), 3.1415);

    auto neg_iter = ondemand::parser::Iterator(iter).findKey("negative");
    ASSERT_TRUE(neg_iter.has_value());
    auto neg_val = neg_iter.value().getNumber();
    ASSERT_TRUE(neg_val.has_value());
    EXPECT_DOUBLE_EQ(neg_val.value(), -10.0);
}

TEST(IteratorTest, FindKeyNotFound) {
    std::string_view json = R"({"name": "Zuu"})";
    auto scanner_res = ondemand::parser::Scanner(json);
    ASSERT_TRUE(scanner_res.has_value());
    
    auto indices = scanner_res.value();
    ondemand::parser::Iterator iter(indices, json);

    auto not_found = iter.findKey("missing");
    ASSERT_FALSE(not_found.has_value());
    EXPECT_EQ(not_found.error(), Error::KeyNotFound);
}

TEST(IteratorTest, SkipCurrent) {
    std::string_view json = R"([1, {"nested": "value"}, 3])";
    auto scanner_res = ondemand::parser::Scanner(json);
    ASSERT_TRUE(scanner_res.has_value());
    
    auto indices = scanner_res.value();
    ondemand::parser::Iterator iter(indices, json);
    
    ASSERT_EQ(iter.type().value(), constants::JsonType::Array);
    
    // In Array, iter points to '['
    iter.skipCurrent(); // should skip array and be at EOF
    EXPECT_FALSE(iter.type().has_value()); // EOF
}
