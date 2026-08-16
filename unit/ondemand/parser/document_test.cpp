#include <gtest/gtest.h>
#include "zuu_json/ondemand/document.hpp"

using namespace zuu::json;

TEST(DocumentTest, ValidDocument) {
    std::string_view json = R"({"key": "value"})";
    ondemand::Document doc(json);
    
    EXPECT_EQ(doc.error(), Error::None);
    
    auto root_res = doc.root();
    ASSERT_TRUE(root_res.has_value());
    
    auto val_res = doc["key"];
    ASSERT_TRUE(val_res.has_value());
    
    auto str_res = val_res.value().getUnescapedString();
    ASSERT_TRUE(str_res.has_value());
    EXPECT_EQ(str_res.value(), "value");
}

TEST(DocumentTest, EscapedStringDocument) {
    std::string_view json = R"({"desc": "Line1\nLine2\t\"Quotes\""})";
    ondemand::Document doc(json);
    
    EXPECT_EQ(doc.error(), Error::None);
    
    auto desc_res = doc["desc"];
    ASSERT_TRUE(desc_res.has_value());
    
    auto desc_val = desc_res.value();
    
    auto raw_res = doc["desc"].value().getString();
    ASSERT_TRUE(raw_res.has_value());
    EXPECT_EQ(raw_res.value(), "Line1\\nLine2\\t\\\"Quotes\\\"");

    // getUnescapedString() returns parsed string with escapes handled
    auto parsed_res = doc["desc"].value().getUnescapedString();
    ASSERT_TRUE(parsed_res.has_value());
    EXPECT_EQ(parsed_res.value(), "Line1\nLine2\t\"Quotes\"");
}

TEST(DocumentTest, NestedDocument) {
    std::string_view json = R"({"user": {"age": 25}})";
    ondemand::Document doc(json);
    
    EXPECT_EQ(doc.error(), Error::None);
    
    auto user_res = doc["user"];
    ASSERT_TRUE(user_res.has_value());
    
    auto age_res = user_res.value()["age"];
    ASSERT_TRUE(age_res.has_value());

    auto age_val = age_res.value().getNumber();
    ASSERT_TRUE(age_val.has_value());
    EXPECT_DOUBLE_EQ(age_val.value(), 25.0);
}

TEST(DocumentTest, LiteralDocument) {
    std::string_view json = R"({"isValid": true, "isDeleted": false, "pointer": null})";
    ondemand::Document doc(json);
    
    EXPECT_EQ(doc.error(), Error::None);
    
    auto valid_res = doc["isValid"];
    ASSERT_TRUE(valid_res.has_value());
    auto valid_val = valid_res.value().getBool();
    ASSERT_TRUE(valid_val.has_value());
    EXPECT_EQ(valid_val.value(), true);

    auto deleted_res = doc["isDeleted"];
    ASSERT_TRUE(deleted_res.has_value());
    auto deleted_val = deleted_res.value().getBool();
    ASSERT_TRUE(deleted_val.has_value());
    EXPECT_EQ(deleted_val.value(), false);

    auto ptr_res = doc["pointer"];
    ASSERT_TRUE(ptr_res.has_value());
    auto ptr_val = ptr_res.value().getNull();
    ASSERT_TRUE(ptr_val.has_value());
    EXPECT_EQ(ptr_val.value(), nullptr);
}

TEST(DocumentTest, EmptyDocument) {
    std::string_view json = "";
    ondemand::Document doc(json);
    
    EXPECT_NE(doc.error(), Error::None);
    EXPECT_EQ(doc.error(), Error::EmptyValue);
    
    auto root_res = doc.root();
    EXPECT_FALSE(root_res.has_value());
    EXPECT_EQ(root_res.error(), Error::EmptyValue);
}

TEST(DocumentTest, WhitespaceOnlyDocument) {
    std::string_view json = "   ";
    ondemand::Document doc(json);
    
    EXPECT_EQ(doc.error(), Error::None); 
    
    auto root_res = doc.root();
    EXPECT_FALSE(root_res.has_value());
    EXPECT_EQ(root_res.error(), Error::EmptyValue); 
}
