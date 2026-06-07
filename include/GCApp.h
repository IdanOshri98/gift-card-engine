#ifndef GIFT_CARD_APP_H
#define GIFT_CARD_APP_H

#include "Wallet.h"
#include "FileRepository.h"

class GCApp {
private:
    Wallet wallet;
    FileRepository repo;

    void showMenu();
    void handleChoice(int choice);
    void addCard();
    void showCards();
    void editCard();

public:
    GCApp();
    void run();
};

#endif
