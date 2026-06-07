#include "../include/RiskAnalyzer.h"

std::vector<GiftCard> RiskAnalyzer::getAtRiskCards(std::vector<GiftCard>& cards) {
    std::vector<GiftCard> result;
    for (GiftCard& card : cards) {
        if (card.isExpiringSoon() && card.getBalance() > 0) {
            result.push_back(card);
        }
    }
    return result;
}

double RiskAnalyzer::getTotalAtRiskBalance(std::vector<GiftCard>& cards) {
    double total = 0.0;
    for (GiftCard& card : cards) {
        if (card.isExpiringSoon() && card.getBalance() > 0) {
            total += card.getBalance();
        }
    }
    return total;
}
