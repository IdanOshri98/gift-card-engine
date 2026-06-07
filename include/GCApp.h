#ifndef GIFT_CARD_APP_H
#define GIFT_CARD_APP_H

#include "Wallet.h"
#include "FileRepository.h"
#include "CommandParser.h"

class GCApp {
private:
    Wallet wallet;
    FileRepository repo;
    CommandParser parser;

    void showMenu();

public:
    GCApp();
    void run();
};

#endif
