#ifndef RISKANALYZER_H
#define RISKANALYZER_H

#include <vector>
#include "GiftCard.h"

class RiskAnalyzer {
public:
    // Cards expiring within 30 days with balance > 0 — at risk of losing value
    static std::vector<GiftCard> getAtRiskCards(std::vector<GiftCard>& cards);

    // Total balance sitting in at-risk cards
    static double getTotalAtRiskBalance(std::vector<GiftCard>& cards);
};

#endif
