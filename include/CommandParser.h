#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include "Wallet.h"
#include "FileRepository.h"

class CommandParser {
public:
    CommandParser(Wallet& wallet, FileRepository& repo);
    void execute(int choice);

private:
    Wallet& wallet;
    FileRepository& repo;

    void addCard();
    void showCards();
    void editCard();
};

#endif
