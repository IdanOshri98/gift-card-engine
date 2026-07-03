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

TEST(WalletTest, GetCardsByCompanyReflectsBalanceUpdatesAfterAdd) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030", {"Steam"});
    wallet.addCard(card);

    GiftCard* found = wallet.findCardById(1);
    ASSERT_NE(found, nullptr);
    found->deductBalance(40.0);

    auto steamCards = wallet.getCardsByCompany("Steam");
    ASSERT_EQ(steamCards.size(), 1);
    EXPECT_DOUBLE_EQ(steamCards[0].getBalance(), 60.0);
}

TEST(WalletTest, GetCardsByCompanyReflectsTitleUpdatesAfterAdd) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030", {"Steam"});
    wallet.addCard(card);

    GiftCard* found = wallet.findCardById(1);
    ASSERT_NE(found, nullptr);
    found->setTitle("RenamedCard");

    auto steamCards = wallet.getCardsByCompany("Steam");
    ASSERT_EQ(steamCards.size(), 1);
    EXPECT_EQ(steamCards[0].getTitle(), "RenamedCard");
}

TEST(WalletTest, CardWithMultipleCompaniesAppearsInEachBucket) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030", {"Amazon", "Fox"});
    wallet.addCard(card);

    EXPECT_EQ(wallet.getCardsByCompany("Amazon").size(), 1);
    EXPECT_EQ(wallet.getCardsByCompany("Fox").size(), 1);
}

TEST(WalletTest, RemoveCardRemovesItFromAllItsCompanyBuckets) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030", {"Amazon", "Fox"});
    wallet.addCard(card);
    wallet.removeCard(card);

    EXPECT_EQ(wallet.getCardsByCompany("Amazon").size(), 0);
    EXPECT_EQ(wallet.getCardsByCompany("Fox").size(), 0);
}

TEST(WalletTest, RemoveCardDoesNotAffectOtherCardsInSameCompany) {
    Wallet wallet;
    auto c1 = makeCard(1, 100.0, "01-01-2030", {"Amazon"});
    auto c2 = makeCard(2, 200.0, "01-01-2030", {"Amazon"});
    wallet.addCard(c1);
    wallet.addCard(c2);
    wallet.removeCard(c1);

    auto amazonCards = wallet.getCardsByCompany("Amazon");
    ASSERT_EQ(amazonCards.size(), 1);
    EXPECT_EQ(amazonCards[0].getId(), 2);
}

TEST(WalletTest, AddCompanyToCardAddsItToNewCompanyBucket) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030", {"Amazon"});
    wallet.addCard(card);

    GiftCard* found = wallet.findCardById(1);
    ASSERT_NE(found, nullptr);
    wallet.addCompanyToCard(found, "Fox");

    EXPECT_EQ(wallet.getCardsByCompany("Fox").size(), 1);
    EXPECT_EQ(wallet.getCardsByCompany("Amazon").size(), 1);
}

TEST(WalletTest, AddingSameCompanyTwiceDoesNotDuplicateInBucket) {
    Wallet wallet;
    auto card = makeCard(1, 100.0, "01-01-2030", {"Amazon"});
    wallet.addCard(card);

    GiftCard* found = wallet.findCardById(1);
    ASSERT_NE(found, nullptr);
    wallet.addCompanyToCard(found, "Amazon");

    EXPECT_EQ(wallet.getCardsByCompany("Amazon").size(), 1);
}
