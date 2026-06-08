#ifndef REDEMPTIONPLANNER_H
#define REDEMPTIONPLANNER_H

#include <vector>
#include "GiftCard.h"

class RedemptionPlanner {
public:
    // Returns cards sorted by expiry - use the soonest-expiring card first
    static std::vector<GiftCard> planByExpiry(std::vector<GiftCard> cards);

    // Returns cards sorted by balance - use the lowest-balance card first
    static std::vector<GiftCard> planByBalance(std::vector<GiftCard> cards);
};

#endif
