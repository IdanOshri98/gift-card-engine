#include <gtest/gtest.h>
#include "GiftCard.h"

static GiftCard makeCard(double balance, const std::string& expiry) {
    int id = 1;
    std::string title = "TestCard";
    std::vector<std::string> companies = {"Company"};
    return GiftCard(id, title, companies, balance, expiry);
}

TEST(GiftCardTest, GettersReturnCorrectValues) {
    auto card = makeCard(100.0, "01-01-2030");
    EXPECT_EQ(card.getId(), 1);
    EXPECT_EQ(card.getTitle(), "TestCard");
    EXPECT_DOUBLE_EQ(card.getBalance(), 100.0);
    EXPECT_EQ(card.getExpiryDate(), "01-01-2030");
}

TEST(GiftCardTest, AddBalanceIncreasesBalance) {
    auto card = makeCard(100.0, "01-01-2030");
    card.addBalance(50.0);
    EXPECT_DOUBLE_EQ(card.getBalance(), 150.0);
}

TEST(GiftCardTest, DeductBalanceDecreasesBalance) {
    auto card = makeCard(100.0, "01-01-2030");
    card.deductBalance(30.0);
    EXPECT_DOUBLE_EQ(card.getBalance(), 70.0);
}

TEST(GiftCardTest, DeductBalanceThrowsWhenInsufficientFunds) {
    auto card = makeCard(50.0, "01-01-2030");
    EXPECT_THROW(card.deductBalance(100.0), std::runtime_error);
}

TEST(GiftCardTest, SetBalanceUpdatesBalance) {
    auto card = makeCard(100.0, "01-01-2030");
    card.setBalance(200.0);
    EXPECT_DOUBLE_EQ(card.getBalance(), 200.0);
}

TEST(GiftCardTest, SetTitleUpdatesTitle) {
    auto card = makeCard(100.0, "01-01-2030");
    card.setTitle("NewTitle");
    EXPECT_EQ(card.getTitle(), "NewTitle");
}

TEST(GiftCardTest, IsExpiredReturnsTrueForPastDate) {
    auto card = makeCard(100.0, "01-01-2020");
    EXPECT_TRUE(card.isExpired());
}

TEST(GiftCardTest, IsExpiredReturnsFalseForFutureDate) {
    auto card = makeCard(100.0, "01-01-2030");
    EXPECT_FALSE(card.isExpired());
}

TEST(GiftCardTest, IsExpiringSoonReturnsTrueWithin30Days) {
    auto card = makeCard(100.0, "20-06-2026");
    EXPECT_TRUE(card.isExpiringSoon());
}

TEST(GiftCardTest, IsExpiringSoonReturnsFalseForFarFuture) {
    auto card = makeCard(100.0, "01-01-2030");
    EXPECT_FALSE(card.isExpiringSoon());
}

TEST(GiftCardTest, IsExpiringSoonReturnsFalseForExpiredCard) {
    auto card = makeCard(100.0, "01-01-2020");
    EXPECT_FALSE(card.isExpiringSoon());
}

TEST(GiftCardTest, AddCompanyAddsNewCompany) {
    auto card = makeCard(100.0, "01-01-2030");
    card.addCompany("NewCompany");
    auto companies = card.getCompanies();
    ASSERT_EQ(companies.size(), 2);
    EXPECT_EQ(companies[0], "Company");
    EXPECT_EQ(companies[1], "NewCompany");
}

TEST(GiftCardTest, AddCompanyDoesNotDuplicateExistingCompany) {
    auto card = makeCard(100.0, "01-01-2030");
    card.addCompany("Company");
    EXPECT_EQ(card.getCompanies().size(), 1);
}
