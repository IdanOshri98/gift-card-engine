#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <vector>
#include "GiftCard.h"

class IRepository {
public:
    virtual void save(const std::vector<GiftCard>& cards) = 0;
    virtual std::vector<GiftCard> load() = 0;
    virtual ~IRepository() = default;
};

#endif
