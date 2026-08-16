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
    
    auto str_res = val_res.value().getString();
    ASSERT_TRUE(str_res.has_value());
    EXPECT_EQ(str_res.value(), "value");
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
