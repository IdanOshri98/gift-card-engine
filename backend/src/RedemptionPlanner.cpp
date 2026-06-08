#include <algorithm>
#include "../include/RedemptionPlanner.h"

std::vector<GiftCard> RedemptionPlanner::planByExpiry(std::vector<GiftCard> cards) {
    std::sort(cards.begin(), cards.end(), [](GiftCard& a, GiftCard& b) {
        a.updateDaysLeft();
        b.updateDaysLeft();
        return a.getDaysToExpiry() < b.getDaysToExpiry();
    });
    return cards;
}

std::vector<GiftCard> RedemptionPlanner::planByBalance(std::vector<GiftCard> cards) {
    std::sort(cards.begin(), cards.end(), [](const GiftCard& a, const GiftCard& b) {
        return a.getBalance() < b.getBalance();
    });
    return cards;
}
