#include <gtest/gtest.h>
#include "Wallet.h"
#include "GiftCard.h"

static GiftCard makeCard(int id, double balance, const std::string& expiry,
                          const std::vector<std::string>& companies = {"Company"}) {
    std::string title = "Card" + std::to_string(id);
    std::vector<std::string> comp = companies;
    return GiftCard(id, title, comp, balance, expiry);
}

TEST(WalletTest, AddCardIncreasesSize) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030");
    wallet.addCard(card);
    EXPECT_EQ(wallet.size(), 1);
}

TEST(WalletTest, GetCardsReturnsAllCards) {
    Wallet wallet;
    auto c1 = makeCard(1, 100.0, "01-01-2030");
    auto c2 = makeCard(2, 200.0, "01-01-2030");
    wallet.addCard(c1);
    wallet.addCard(c2);
    EXPECT_EQ(wallet.getCards().size(), 2);
}

TEST(WalletTest, RemoveCardDecreasesSize) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030");
    wallet.addCard(card);
    wallet.removeCard(card);
    EXPECT_EQ(wallet.size(), 0);
}

TEST(WalletTest, FindCardByIdReturnsCorrectCard) {
    Wallet wallet;
    auto card = makeCard(42, 100.0, "01-01-2030");
    wallet.addCard(card);
    GiftCard* found = wallet.findCardById(42);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getId(), 42);
}

TEST(WalletTest, FindCardByIdReturnsNullForMissingId) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030");
    wallet.addCard(card);
    EXPECT_EQ(wallet.findCardById(99), nullptr);
}

TEST(WalletTest, GetExpiredCardsReturnsOnlyExpired) {
    Wallet wallet;
    auto expired = makeCard(1, 100.0, "01-01-2020");
    auto active  = makeCard(2, 100.0, "01-01-2030");
    wallet.addCard(expired);
    wallet.addCard(active);
    EXPECT_EQ(wallet.getExpiredCards().size(), 1);
    EXPECT_EQ(wallet.getExpiredCards()[0].getId(), 1);
}

TEST(WalletTest, GetSoonToExpireReturnsCorrectCards) {
    Wallet wallet;
    auto soon   = makeCard(1, 100.0, "20-06-2026");
    auto later  = makeCard(2, 100.0, "01-01-2030");
    wallet.addCard(soon);
    wallet.addCard(later);
    EXPECT_EQ(wallet.getSoonToExpireCards().size(), 1);
    EXPECT_EQ(wallet.getSoonToExpireCards()[0].getId(), 1);
}

TEST(WalletTest, GetCardsByCompanyReturnsCorrectCards) {
    Wallet wallet;
    auto c1 = makeCard(1, 100.0, "01-01-2030", {"Amazon", "Fox"});
    auto c2 = makeCard(2, 200.0, "01-01-2030", {"Steam"});
    wallet.addCard(c1);
    wallet.addCard(c2);
    EXPECT_EQ(wallet.getCardsByCompany("Amazon").size(), 1);
    EXPECT_EQ(wallet.getCardsByCompany("Steam").size(), 1);
    EXPECT_EQ(wallet.getCardsByCompany("Unknown").size(), 0);
}
