#include <iostream>
#include <vector>
#include <string>

#include "../include/CommandParser.h"

CommandParser::CommandParser(Wallet& wallet, FileRepository& repo)
    : wallet(wallet), repo(repo) {}

void CommandParser::execute(int choice) {
    switch (choice) {
        case 1: addCard();     break;
        case 2: showCards();   break;
        case 3: editCard();    break;
        case 4: removeCard();  break;
        case 5: planCards();   break;
        case 6: analyzeRisk(); break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
    }
}

void CommandParser::addCard() {
    int id = wallet.getNextId();
    std::string title;
    double balance;
    std::string expiryDate;

    std::cout << "Enter title: \n";
    std::cin >> title;

    std::cout << "Enter balance: \n";
    std::cin >> balance;
    if (balance <= 0) {
        std::cout << "Balance must be greater than 0. Cancelled.\n";
        return;
    }

    std::cout << "Enter expiry date (DD-MM-YYYY): \n";
    std::cin >> expiryDate;

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
            if (operation != '+' && operation != '-' && operation != '=') {
                std::cout << "Invalid operation. Use +, - or =.\n";
                break;
            }
            std::cout << "Enter amount: \n";
            double amount;
            std::cin >> amount;
            if (amount <= 0) {
                std::cout << "Amount must be greater than 0.\n";
                break;
            }
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

void CommandParser::removeCard() {
    std::cout << "Enter card ID to remove: \n";
    int id;
    std::cin >> id;

    GiftCard* card = wallet.findCardById(id);
    if (card == nullptr) {
        std::cout << "Card not found.\n";
        return;
    }

    card->updateDaysLeft();
    std::cout << "Card to remove:\n"
              << "  ID: "          << card->getId()
              << " | Title: "      << card->getTitle()
              << " | Balance: "    << card->getBalance()
              << " | Expiry Date: "<< card->getExpiryDate()
              << " | Days Left: "  << card->getDaysToExpiry()
              << std::endl;

    std::cout << "Are you sure you want to remove this card? (y/n): \n";
    char confirm;
    std::cin >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Cancelled." << std::endl;
        return;
    }

    wallet.removeCard(*card);
    repo.save(wallet.getCards());
    std::cout << "Card removed successfully!" << std::endl;
}

void CommandParser::planCards() {
    std::cout << "Plan redemption by:\n"
              << "1. expiry date (use soonest-expiring first)\n"
              << "2. balance (use lowest balance first)\n";

    int choice;
    std::cin >> choice;

    std::vector<GiftCard> cards = wallet.getCards();
    std::vector<GiftCard> planned;

    if (choice == 1) {
        planned = RedemptionPlanner::planByExpiry(cards);
        std::cout << "Recommended order (by expiry):" << std::endl;
    } else if (choice == 2) {
        planned = RedemptionPlanner::planByBalance(cards);
        std::cout << "Recommended order (by balance):" << std::endl;
    } else {
        std::cout << "Invalid choice." << std::endl;
        return;
    }

    int rank = 1;
    for (GiftCard& card : planned) {
        card.updateDaysLeft();
        std::cout << rank++ << ". "
                  << card.getTitle()
                  << " | Balance: " << card.getBalance()
                  << " | Days Left: " << card.getDaysToExpiry()
                  << std::endl;
    }
}

void CommandParser::analyzeRisk() {
    std::vector<GiftCard> cards = wallet.getCards();
    std::vector<GiftCard> atRisk = RiskAnalyzer::getAtRiskCards(cards);
    double totalRisk = RiskAnalyzer::getTotalAtRiskBalance(cards);

    if (atRisk.empty()) {
        std::cout << "No at-risk cards." << std::endl;
        return;
    }

    std::cout << "At-risk cards (expiring within 30 days with remaining balance):" << std::endl;
    for (GiftCard& card : atRisk) {
        card.updateDaysLeft();
        std::cout << "  " << card.getTitle()
                  << " | Balance: " << card.getBalance()
                  << " | Days Left: " << card.getDaysToExpiry()
                  << std::endl;
    }
    std::cout << "Total at-risk balance: " << totalRisk << std::endl;
}
