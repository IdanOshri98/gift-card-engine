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
        cardsByCompany[company].insert(card.getId());
    }
}

void Wallet :: removeCard(GiftCard& card) {
    for (const std::string& company : card.getCompanies()) {
        auto it = cardsByCompany.find(company);
        if (it != cardsByCompany.end()) {
            it->second.erase(card.getId());
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
    std::vector<GiftCard> result;
    auto it = cardsByCompany.find(company);
    if (it == cardsByCompany.end()) {
        return result;
    }

    for (const GiftCard& card : cards) {
        if (it->second.count(card.getId())) {
            result.push_back(card);
        }
    }
    return result;
}

void Wallet::addCompanyToCard(GiftCard* card, const std::string& company) {
    card->addCompany(company);
    cardsByCompany[company].insert(card->getId());
}

void Wallet::syncCompaniesForCard(GiftCard* card, const std::vector<std::string>& newCompanies) {
    // הסר את הכרטיס מכל bucket קיים
    for (const std::string& company : card->getCompanies()) {
        auto it = cardsByCompany.find(company);
        if (it != cardsByCompany.end()) {
            it->second.erase(card->getId());
            if (it->second.empty()) cardsByCompany.erase(it);
        }
    }
    // החלף את רשימת החברות בכרטיס עצמו
    card->setCompanies(newCompanies);
    // הוסף לbuckets החדשים
    for (const std::string& company : newCompanies) {
        cardsByCompany[company].insert(card->getId());
    }
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
