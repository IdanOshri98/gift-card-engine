#include <iostream>
#include <limits>
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
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

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
              << " 4. remove card\n"
              << " 5. plan redemption\n"
              << " 6. analyze risk\n"
              << " 0. exit\n";
}
