#include <iostream>
#include <vector>
#include <string>

#include "../include/GCApp.h"

using namespace std;

GCApp::GCApp() : repo("cards.txt"), parser(wallet, repo) {}

void GCApp::run() {
    std::cout << "Gift Card Engine running..." << std::endl;

    std::vector<GiftCard> loaded = repo.load();
    for (GiftCard& card : loaded) {
        wallet.addCard(card);
    }

    while (true) {
        showMenu();

        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Exiting..." << std::endl;
            break;
        }

        parser.execute(choice);
    }
}

void GCApp::showMenu() {
    std::vector<GiftCard> cards = wallet.getCards();

    if (!cards.empty()) {
        std::cout << "today's date is: " << cards[0].getTodayDate() << std::endl;
    }

    std::cout << "\n Choose an option:\n"
              << " 1. add card\n"
              << " 2. show cards\n"
              << " 3. edit card\n"
              << " 0. exit\n";
}
