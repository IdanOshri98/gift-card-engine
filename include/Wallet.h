#include <string>
#include <vector>
#include "GiftCard.h"
#include <unordered_map>

class Wallet{
    public:
        Wallet();

        void addCard(GiftCard& card);
        void removeCard(GiftCard& card);
        void addCardToSoonToExpire(GiftCard& card);
        void removeCardFromSoonToExpire(GiftCard& card);
        void addCardToExpired(GiftCard& card);
        GiftCard* findCardById(int id);

        const std::vector<GiftCard> getCards() const;
        std::vector<GiftCard> getExpiredCards() const;
        std::vector<GiftCard> getSoonToExpireCards() const;
        std::vector<GiftCard> getCardsByCompany(const std::string& company) const;
        int size() ;

    private:
        std::vector<GiftCard> cards;
        std::vector<GiftCard> soonToExpireCards;
        std::vector<GiftCard> expiredCards;
        std::unordered_map<std::string, std::vector<GiftCard>> cardsByCompany;






};