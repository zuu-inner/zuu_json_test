/**
 * @file main.cpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Unit tests for Error string conversions.
 * @version 0.2.0
 * @date 2026-07-28
 *
 * @copyright Copyright (c) 2026
 */

#include <cstring>
#include <gtest/gtest.h>
#include <zuu_json/error.hpp>
#include <zuu_json/version.hpp>

TEST(JsonErrorTest, ResolveError) {
    using zuu::json::Error;
    using zuu::json::ResolveError;

    EXPECT_STREQ(ResolveError(Error::None), "No error");
    EXPECT_STREQ(ResolveError(Error::CommentNotAllowed),
                 "Comments are not allowed in standard JSON");
    EXPECT_STREQ(ResolveError(Error::DepthLimitExceeded), "Maximum nesting depth limit exceeded");
    EXPECT_STREQ(ResolveError(Error::EmptyValue), "Empty JSON input or value");
    EXPECT_STREQ(ResolveError(Error::InvalidBooleanLiteral), "Invalid boolean literal");
    EXPECT_STREQ(ResolveError(Error::InvalidFormat), "Invalid JSON format");
    EXPECT_STREQ(ResolveError(Error::InvalidNullLiteral), "Invalid null literal");
    EXPECT_STREQ(ResolveError(Error::InvalidSurrogate), "Invalid UTF-16 surrogate pair");
    EXPECT_STREQ(ResolveError(Error::InvalidType), "Invalid or unexpected JSON type");
    EXPECT_STREQ(ResolveError(Error::InvalidUnicode), "Invalid UTF-8/Unicode sequence");
    EXPECT_STREQ(ResolveError(Error::InvalidValue), "Invalid JSON value");
    EXPECT_STREQ(ResolveError(Error::IsNotArray), "JSON element is not an array");
    EXPECT_STREQ(ResolveError(Error::IsNotObject), "JSON element is not an object");
    EXPECT_STREQ(ResolveError(Error::LeadingZero), "Numbers cannot have leading zeros");
    EXPECT_STREQ(ResolveError(Error::MissingComma), "Missing expected comma separator");
    EXPECT_STREQ(ResolveError(Error::OutOfBound), "Index or value out of bounds");
    EXPECT_STREQ(ResolveError(Error::RootNotArrayType), "Root JSON element is not an array");
    EXPECT_STREQ(ResolveError(Error::RootNotObjectType), "Root JSON element is not an object");
    EXPECT_STREQ(ResolveError(Error::SingleQuotedString), "Strings must use double quotes");
    EXPECT_STREQ(ResolveError(Error::TrailingComma), "Trailing commas are not allowed");
    EXPECT_STREQ(ResolveError(Error::UnescapedCharacter), "Unescaped control character in string");
    EXPECT_STREQ(ResolveError(Error::UnquotedKey), "Object keys must be double-quoted strings");
    EXPECT_STREQ(ResolveError(Error::Unknown), "Unknown JSON error");
    EXPECT_STREQ(ResolveError(static_cast<Error>(255)), "Unknown JSON error");
}

TEST(JsonVersionTest, VersionConstants) {
    EXPECT_STREQ(zuu::json::zuu_json_version, "0.2.0");
    EXPECT_EQ(zuu::json::zuu_json_version_major, 0);
    EXPECT_EQ(zuu::json::zuu_json_version_minor, 2);
    EXPECT_EQ(zuu::json::zuu_json_version_patch, 0);
}