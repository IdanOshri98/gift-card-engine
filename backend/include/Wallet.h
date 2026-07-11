#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include "GiftCard.h"
#include <unordered_map>
#include <unordered_set>

class Wallet{
    public:
        Wallet();

        void addCard(GiftCard& card);
        void removeCard(GiftCard& card);
        void addCompanyToCard(GiftCard* card, const std::string& company);
        void syncCompaniesForCard(GiftCard* card, const std::vector<std::string>& newCompanies);
        GiftCard* findCardById(int id);

        const std::vector<GiftCard> getCards() const;
        std::vector<GiftCard> getExpiredCards();
        std::vector<GiftCard> getSoonToExpireCards();
        std::vector<GiftCard> getCardsByCompany(const std::string& company) const;
        int size();
        int getNextId() const;

    private:
        std::vector<GiftCard> cards;
        std::unordered_map<std::string, std::unordered_set<int>> cardsByCompany; // company -> card IDs

};

#endif
