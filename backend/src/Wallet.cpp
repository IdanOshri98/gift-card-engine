#include <vector>
#include <string>
#include <algorithm>

#include "../include/Wallet.h"
#include "../include/GiftCard.h"

Wallet :: Wallet() {
    cards = std::vector<GiftCard>();
}

void Wallet :: addCard(GiftCard& card){
    cards.push_back(card);
    for(const std::string& company : card.getCompanies()) {
        cardsByCompany[company].push_back(card);
    }
}

void Wallet :: removeCard(GiftCard& card) {
    for (const std::string& company : card.getCompanies()) {
        auto it = cardsByCompany.find(company);
        if (it != cardsByCompany.end()) {
            it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [&](const GiftCard& c) {
                return c.getId() == card.getId();
            }), it->second.end());
            if (it->second.empty()) {
                cardsByCompany.erase(it);
            }
        }
    }

    cards.erase(std::remove_if(cards.begin(), cards.end(), [&](const GiftCard& c) {
        return c.getId() == card.getId();
    }), cards.end());
}

const std::vector<GiftCard> Wallet :: getCards() const {
    return cards;
}

int Wallet :: size() {
    return cards.size();
}

std::vector<GiftCard> Wallet :: getExpiredCards() {
    std::vector<GiftCard> result;
    for (GiftCard& card : cards) {
        if (card.isExpired()) {
            result.push_back(card);
        }
    }
    return result;
}

std::vector<GiftCard> Wallet :: getSoonToExpireCards() {
    std::vector<GiftCard> result;
    for (GiftCard& card : cards) {
        if (card.isExpiringSoon()) {
            result.push_back(card);
        }
    }
    return result;
}

std::vector<GiftCard> Wallet :: getCardsByCompany(const std::string& company) const {
    auto it = cardsByCompany.find(company);
    if(it != cardsByCompany.end()) {
        return it->second;
    }
    return {};
}

void Wallet::addCompanyToCard(GiftCard* card, const std::string& company) {
    card->addCompany(company);
    cardsByCompany[company].push_back(*card);
}

int Wallet::getNextId() const {
    int maxId = 0;
    for (const GiftCard& card : cards) {
        if (card.getId() > maxId) maxId = card.getId();
    }
    return maxId + 1;
}

GiftCard* Wallet::findCardById(int id) {
    for (GiftCard& card : cards) {
        if (card.getId() == id) {
            return &card;
        }
    }
    return nullptr;
}
