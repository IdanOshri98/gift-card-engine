#include <gtest/gtest.h>
#include "RiskAnalyzer.h"
#include "GiftCard.h"
#include <ctime>
#include <sstream>
#include <iomanip>

static GiftCard makeCard(int id, double balance, const std::string& expiry) {
    std::string title = "Card" + std::to_string(id);
    std::vector<std::string> companies = {"Company"};
    return GiftCard(id, title, companies, balance, expiry);
}

static std::string dateInDays(int n) {
    std::time_t t = std::time(nullptr) + static_cast<std::time_t>(n) * 86400;
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << tm->tm_mday << "-"
        << std::setw(2) << std::setfill('0') << (tm->tm_mon + 1) << "-"
        << (tm->tm_year + 1900);
    return oss.str();
}

TEST(RiskAnalyzerTest, DetectsAtRiskCards) {
    std::vector<GiftCard> cards = {
        makeCard(1, 100.0, dateInDays(15)),  // expiring soon - at risk
        makeCard(2, 200.0, "01-01-2030")     // far future - not at risk
    };

    auto atRisk = RiskAnalyzer::getAtRiskCards(cards);

    ASSERT_EQ(atRisk.size(), 1);
    EXPECT_EQ(atRisk[0].getId(), 1);
}

TEST(RiskAnalyzerTest, ZeroBalanceCardIsNotAtRisk) {
    std::vector<GiftCard> cards = {
        makeCard(1, 0.0, dateInDays(15))  // expiring soon but no balance
    };

    auto atRisk = RiskAnalyzer::getAtRiskCards(cards);

    EXPECT_TRUE(atRisk.empty());
}

TEST(RiskAnalyzerTest, ExpiredCardIsNotAtRisk) {
    std::vector<GiftCard> cards = {
        makeCard(1, 100.0, "01-01-2020")  // already expired
    };

    auto atRisk = RiskAnalyzer::getAtRiskCards(cards);

    EXPECT_TRUE(atRisk.empty());
}

TEST(RiskAnalyzerTest, TotalAtRiskBalanceIsCorrect) {
    std::vector<GiftCard> cards = {
        makeCard(1, 100.0, dateInDays(15)),
        makeCard(2, 50.0,  dateInDays(20)),
        makeCard(3, 200.0, "01-01-2030")
    };

    double total = RiskAnalyzer::getTotalAtRiskBalance(cards);

    EXPECT_DOUBLE_EQ(total, 150.0);
}

TEST(RiskAnalyzerTest, NoAtRiskCardsReturnsEmpty) {
    std::vector<GiftCard> cards = {
        makeCard(1, 100.0, "01-01-2030"),
        makeCard(2, 200.0, "01-01-2029")
    };

    auto atRisk = RiskAnalyzer::getAtRiskCards(cards);

    EXPECT_TRUE(atRisk.empty());
}
