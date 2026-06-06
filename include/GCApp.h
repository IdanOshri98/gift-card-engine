#ifndef GIFT_CARD_APP_H
#define GIFT_CARD_APP_H

#include "Wallet.h"

class GCApp {
private:
    Wallet wallet;

    void showMenu() ;
    void handleChoice(int choice);
    void addCard();
    void showCards();
    void editCard();

public:
    void run();
};

#endif