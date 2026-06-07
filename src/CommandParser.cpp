#include <iostream>
#include <vector>
#include <string>

#include "../include/CommandParser.h"

CommandParser::CommandParser(Wallet& wallet, FileRepository& repo)
    : wallet(wallet), repo(repo) {}

void CommandParser::execute(int choice) {
    switch (choice) {
        case 1: addCard();   break;
        case 2: showCards(); break;
        case 3: editCard();  break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
    }
}

void CommandParser::addCard() {
    int id = wallet.size() + 1;
    std::string title;
    double balance;
    std::string expiryDate;

    std::cout << "Enter card details (title, balance, expiry date DD-MM-YYYY): \n";
    std::cin >> title >> balance >> expiryDate;

    std::cout << "Enter companies (type 'done' when finished): \n";
    std::string company;
    std::vector<std::string> companies;
    while (std::cin >> company && company != "done") {
        companies.push_back(company);
    }

    GiftCard newCard(id, title, companies, balance, expiryDate);
    wallet.addCard(newCard);
    repo.save(wallet.getCards());
    std::cout << "Card added successfully!" << std::endl;
}

void CommandParser::showCards() {
    std::cout << "choose option: \n"
              << "1. show all cards \n"
              << "2. show soon to expire cards \n"
              << "3. show expired cards \n"
              << "4. show cards by company \n";

    int choice;
    std::cin >> choice;

    std::vector<GiftCard> cards;
    switch (choice) {
        case 1:
            cards = wallet.getCards();
            std::cout << "Cards in wallet:" << std::endl;
            break;
        case 2:
            cards = wallet.getSoonToExpireCards();
            std::cout << "Soon to expire cards:" << std::endl;
            break;
        case 3:
            cards = wallet.getExpiredCards();
            std::cout << "Expired cards:" << std::endl;
            break;
        case 4: {
            std::cout << "Enter company name: \n";
            std::string company;
            std::cin >> company;
            cards = wallet.getCardsByCompany(company);
            std::cout << "Cards by company '" << company << "':" << std::endl;
            break;
        }
        default:
            break;
    }

    for (GiftCard& card : cards) {
        card.updateDaysLeft();
        std::cout << "ID: "            << card.getId()
                  << "| Title: "       << card.getTitle()
                  << "| Balance: "     << card.getBalance()
                  << "| Expiry Date: " << card.getExpiryDate()
                  << "| Days Left: "   << card.getDaysToExpiry()
                  << std::endl;
    }
}

void CommandParser::editCard() {
    std::cout << "Enter card ID to edit: \n";
    int id;
    std::cin >> id;

    GiftCard* card = wallet.findCardById(id);
    if (card == nullptr) {
        std::cout << "Card not found.\n";
        return;
    }

    std::cout << "Editing card with ID: " << id << std::endl;
    std::cout << "what do you want to edit?\n"
                 "1. title\n"
                 "2. balance\n"
                 "3. expiry date\n"
                 "4. companies\n";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1: {
            std::cout << "Enter new title: \n";
            std::string newTitle;
            std::cin >> newTitle;
            card->setTitle(newTitle);
            break;
        }
        case 2: {
            std::cout << "to set new balance type = , to add type + , to subtract type - : \n";
            char operation;
            std::cin >> operation;
            std::cout << "Enter amount: \n";
            double amount;
            std::cin >> amount;
            if (operation == '+') {
                card->addBalance(amount);
            } else if (operation == '-') {
                try {
                    card->deductBalance(amount);
                } catch (const std::runtime_error& e) {
                    std::cout << e.what() << std::endl;
                }
            } else if (operation == '=') {
                card->setBalance(amount);
            }
            break;
        }
        case 3: {
            std::cout << "Enter new expiry date (DD-MM-YYYY): \n";
            std::string newExpiryDate;
            std::cin >> newExpiryDate;
            card->setExpiryDate(newExpiryDate);
            break;
        }
        case 4: {
            std::cout << "Enter companies to add (type 'done' when finished): \n";
            std::string company;
            while (std::cin >> company && company != "done") {
                wallet.addCompanyToCard(card, company);
            }
            break;
        }
        default:
            break;
    }

    repo.save(wallet.getCards());
}
