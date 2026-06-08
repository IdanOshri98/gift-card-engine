#include <gtest/gtest.h>
#include "RedemptionPlanner.h"
#include "GiftCard.h"

static GiftCard makeCard(int id, double balance, const std::string& expiry) {
    std::string title = "Card" + std::to_string(id);
    std::vector<std::string> companies = {"Company"};
    return GiftCard(id, title, companies, balance, expiry);
}

TEST(RedemptionPlannerTest, PlanByExpirySortsAscending) {
    std::vector<GiftCard> cards = {
        makeCard(1, 100.0, "01-01-2030"),
        makeCard(2, 100.0, "01-01-2027"),
        makeCard(3, 100.0, "01-01-2028")
    };

    auto planned = RedemptionPlanner::planByExpiry(cards);

    EXPECT_EQ(planned[0].getId(), 2);
    EXPECT_EQ(planned[1].getId(), 3);
    EXPECT_EQ(planned[2].getId(), 1);
}

TEST(RedemptionPlannerTest, PlanByBalanceSortsAscending) {
    std::vector<GiftCard> cards = {
        makeCard(1, 300.0, "01-01-2030"),
        makeCard(2, 100.0, "01-01-2030"),
        makeCard(3, 200.0, "01-01-2030")
    };

    auto planned = RedemptionPlanner::planByBalance(cards);

    EXPECT_EQ(planned[0].getId(), 2);
    EXPECT_EQ(planned[1].getId(), 3);
    EXPECT_EQ(planned[2].getId(), 1);
}

TEST(RedemptionPlannerTest, PlanDoesNotModifyOriginalVector) {
    std::vector<GiftCard> cards = {
        makeCard(1, 300.0, "01-01-2030"),
        makeCard(2, 100.0, "01-01-2030")
    };

    RedemptionPlanner::planByBalance(cards);

    EXPECT_EQ(cards[0].getId(), 1);
    EXPECT_EQ(cards[1].getId(), 2);
}
