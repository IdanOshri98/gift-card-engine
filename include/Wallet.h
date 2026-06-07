#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include "GiftCard.h"
#include <unordered_map>

class Wallet{
    public:
        Wallet();

        void addCard(GiftCard& card);
        void removeCard(GiftCard& card);
        void addCompanyToCard(GiftCard* card, const std::string& company);
        GiftCard* findCardById(int id);

        const std::vector<GiftCard> getCards() const;
        std::vector<GiftCard> getExpiredCards();
        std::vector<GiftCard> getSoonToExpireCards();
        std::vector<GiftCard> getCardsByCompany(const std::string& company) const;
        int size();

    private:
        std::vector<GiftCard> cards;
        std::unordered_map<std::string, std::vector<GiftCard>> cardsByCompany;

};

#endif
