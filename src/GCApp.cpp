#include <iostream>
#include <vector>
#include <string>

#include "../include/GCApp.h"

using namespace std;

void GCApp :: run() {
    std::cout << "Gift Card Engine running..." << std::endl;

    while(true){
        showMenu();

        int choice;
        cin >> choice;

        if(choice == 0) {
            std:: cout << "Exiting..." << endl;
            break;
        }
        handleChoice(choice);
    }
}

void GCApp :: showMenu() {

    std::vector<GiftCard> cards = wallet.getCards();

    if (cards.empty()) {

        std::string title = "Test Card";

        std::vector<std::string> companies = {
            "Amazon",
            "Fox",
            "Steam"
        };

        double balance = 250.0;

        std::string expiryDate = "31-12-2026";

        GiftCard testCard(
            1,
            title,
            companies,
            balance,
            expiryDate
        );

        wallet.addCard(testCard);

        cards = wallet.getCards();
    }

    std::cout << "today's date is: "
              << cards[0].getTodayDate()
              << std::endl;

    std::cout << " \n Choose an option: \n";
    std::cout << " 1. add card \n";
    std::cout << " 2. show cards \n";
    std::cout << " 3. edit card \n";
    std::cout << " 0. exit  \n";
}



void GCApp :: handleChoice(int choice) {
    switch(choice) {
        case 1:
            addCard();
            break;
        case 2:
            showCards();
            break;
        case 3:
            editCard();
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
    }
}



void GCApp :: addCard() {
    std::cout << "Enter card details ( title, balance, expiry date- DD-MM-YYYY ): \n";
    int id=wallet.size()+1;
    std::string title;
    double balance;
    std::string expiryDate;
    std::cin >> title >> balance >> expiryDate;

    std::cout << "Enter companies (press enter for each - when you done write 'done'): \n";

    std::string company;
    std::vector<std:: string> companies;

    while(std:: cin >> company && company != "done") {
        companies.push_back(company);
    }

    GiftCard newCard(id, title, companies, balance, expiryDate);
    wallet.addCard(newCard);
    std::cout << "Card added successfully!" << std::endl;
}



void GCApp :: showCards() {
    std::vector<GiftCard> cards = wallet.getCards();

    std::cout << "choose option: \n"
              << "1. show all cards \n"
              << "2. show soon to expire cards \n"
         << "3. show expired cards \n"
         << "4. show cards by company \n"
         ;

    int choice;
    std::cin >> choice;
    switch (choice)
    {
        case 1: {
            cards = wallet.getCards();
            std::cout << "Cards in wallet:" << std::endl;
            break;}
        case 2: {
            cards = wallet.getSoonToExpireCards();
            std::cout << "Soon to expire cards:" << std::endl;
            break;}
        case 3: {
            cards = wallet.getExpiredCards();
            std::cout << "Expired cards:" << std::endl;
            break;}
        case 4: {
                    std:: cout << "Enter company name: \n";
                    std::string company;
                    std::cin >> company;
                    
                    cards = wallet.getCardsByCompany(company);
                    std::cout << "Cards by company '" << company << "':" << std::endl;
                    break;
                }
    
        default:
            break;
    }

    

    for(GiftCard& card : cards) {
        card.updateDaysLeft();
        std::cout    << "ID: "            << card.getId() 
                     << "| Title: "       << card.getTitle() 
                     << "| Balance: "     << card.getBalance() 
                    << "| Expiry Date: " << card.getExpiryDate() 
                    << "| Days Left: "   << card.getDaysToExpiry()
                    << std::endl;

        
    }
}

void GCApp :: editCard() {
    std::cout << "Enter card ID to edit: \n";
    int id;
    std::cin >> id;

    // Here you would implement the logic to find the card by ID and allow the user to edit its details.
    // This is a placeholder for demonstration purposes.
    
    GiftCard* card = wallet.findCardById(id);

    if (card == nullptr) {
        std::cout << "Card not found.\n";
        return;
    }

    
    std::cout << "Editing card with ID: " << id << std::endl;
    std::cout << "what do you want to edit? - choose number \n"
                    "1. new title \n"
                    "2. balance \n"
                    "3. expiry date \n"
                    "4. companies \n";
    int choice;
    std::cin >> choice;

    switch (choice)
    {
        case 1: {
            std::cout << "Enter new title: \n";
            std::string newTitle;
            std::cin >> newTitle;
            card->setTitle(newTitle);

            break;
        }


        case 2: {
            std::cout << "to set new balance type = , to add type + and to subtract - : \n";
            char operation;
            std::cin >> operation;
            std::cout << "Enter amount: \n";
            double newBalance;
            std::cin >> newBalance;

            if(operation == '+') {
                card->addBalance(newBalance);
            } else if(operation == '-') {
                        try {
                            card->deductBalance(newBalance);
                        } catch (const std::runtime_error& e) {
                            std::cout << e.what() << std::endl;
                        }
            } else if(operation == '=') {
                        card->setBalance(newBalance);}

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
            std::cout << "Enter new companies (press enter for each - when you done write 'done'): \n";
            std::string company;
            while(std::cin >> company && company != "done") {
                card->addCompany(company);
            }
            break;
        }

        default:
            break;
    }
                        
    



    std::cout << "Editing card with ID: " << id << std::endl;
}