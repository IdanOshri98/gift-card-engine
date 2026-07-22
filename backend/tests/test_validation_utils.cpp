#include <gtest/gtest.h>
#include "ValidationUtils.h"

TEST(ValidationUtilsTest, IsValidExpiryDateAcceptsValidDate) {
    EXPECT_TRUE(ValidationUtils::isValidExpiryDate("31-12-2026"));
}

TEST(ValidationUtilsTest, IsValidExpiryDateRejectsWrongFormat) {
    EXPECT_FALSE(ValidationUtils::isValidExpiryDate("2026-12-31"));
}

TEST(ValidationUtilsTest, IsValidExpiryDateRejectsImpossibleDate) {
    EXPECT_FALSE(ValidationUtils::isValidExpiryDate("31-02-2026"));
}

TEST(ValidationUtilsTest, IsValidExpiryDateRejectsEmptyString) {
    EXPECT_FALSE(ValidationUtils::isValidExpiryDate(""));
}

TEST(ValidationUtilsTest, IsValidExpiryDateRejectsGarbageText) {
    EXPECT_FALSE(ValidationUtils::isValidExpiryDate("not-a-date"));
}

TEST(ValidationUtilsTest, IsValidExpiryDateRejectsTrailingGarbage) {
    EXPECT_FALSE(ValidationUtils::isValidExpiryDate("31-12-2026x"));
}

TEST(ValidationUtilsTest, HasReservedDelimiterDetectsPipe) {
    EXPECT_TRUE(ValidationUtils::hasReservedDelimiter("Amazon|Gift"));
}

TEST(ValidationUtilsTest, HasReservedDelimiterDetectsComma) {
    EXPECT_TRUE(ValidationUtils::hasReservedDelimiter("Amazon,Gift"));
}

TEST(ValidationUtilsTest, HasReservedDelimiterCleanString) {
    EXPECT_FALSE(ValidationUtils::hasReservedDelimiter("Amazon Gift"));
}

TEST(ValidationUtilsTest, HasInvalidDelimitersCleanTitleAndCompanies) {
    EXPECT_FALSE(ValidationUtils::hasInvalidDelimiters("Amazon", {"Amazon Inc", "Whole Foods"}));
}

TEST(ValidationUtilsTest, HasInvalidDelimitersDirtyTitle) {
    EXPECT_TRUE(ValidationUtils::hasInvalidDelimiters("Amazon|Gift", {"Amazon Inc"}));
}

TEST(ValidationUtilsTest, HasInvalidDelimitersOneDirtyCompany) {
    EXPECT_TRUE(ValidationUtils::hasInvalidDelimiters("Amazon", {"Amazon Inc", "Dirty,Company"}));
}

TEST(ValidationUtilsTest, HasInvalidDelimitersAllClean) {
    EXPECT_FALSE(ValidationUtils::hasInvalidDelimiters("Amazon", {"Amazon Inc", "Whole Foods", "Ring"}));
}
