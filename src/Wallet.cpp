#include <vector>
#include <string>
#include <algorithm>

#include "../include/Wallet.h"
#include "../include/GiftCard.h"

Wallet :: Wallet() {
    cards = std::vector<GiftCard>();
    soonToExpireCards = std::vector<GiftCard>();
    expiredCards = std::vector<GiftCard>();

}

void Wallet :: addCard(GiftCard& card){
    cards.push_back(card);
    for(const std::string& company : card.getCompanies()) {
        cardsByCompany[company].push_back(card);
    }
}

void Wallet :: removeCard(GiftCard& card) {
    cards.erase(std::remove_if(cards.begin(), cards.end(), [&](const GiftCard& c) {
        return c.getId() == card.getId();
    }), cards.end());
}

void Wallet :: addCardToSoonToExpire(GiftCard& card) {
    if(card.isExpiringSoon()) {
    soonToExpireCards.push_back(card);}
}

const std::vector<GiftCard> Wallet :: getCards() const {
    return cards;
}

int Wallet :: size()  {
    return cards.size();
}

std::vector<GiftCard> Wallet :: getExpiredCards() const {
    return expiredCards;
}

std::vector<GiftCard> Wallet :: getSoonToExpireCards() const {
    return soonToExpireCards;
}

std::vector<GiftCard> Wallet :: getCardsByCompany(const std::string& company) const {
    auto it = cardsByCompany.find(company);
    if(it != cardsByCompany.end()) {
        return it->second; // Return a vector containing the found card
    }
    return {}; // Return an empty vector if no cards are found
}

GiftCard* Wallet::findCardById(int id) {
    for (GiftCard& card : cards) {
        if (card.getId() == id) {
            return &card;
        }
    }
    return nullptr;
}