#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../include/FileRepository.h"

FileRepository::FileRepository(const std::string& filePath) : filePath(filePath) {}

void FileRepository::save(const std::vector<GiftCard>& cards) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filePath);
    }

    for (const GiftCard& card : cards) {
        file << card.getId() << "|"
             << card.getTitle() << "|"
             << card.getBalance() << "|"
             << card.getExpiryDate() << "|";

        std::vector<std::string> companies = card.getCompanies();
        for (size_t i = 0; i < companies.size(); i++) {
            file << companies[i];
            if (i < companies.size() - 1) file << ",";
        }
        file << "\n";
    }
}

std::vector<GiftCard> FileRepository::load() {
    std::vector<GiftCard> cards;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        return cards;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string idStr, title, balanceStr, expiryDate, companiesStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, title, '|');
        std::getline(ss, balanceStr, '|');
        std::getline(ss, expiryDate, '|');
        std::getline(ss, companiesStr, '|');

        int id = std::stoi(idStr);
        double balance = std::stod(balanceStr);

        std::vector<std::string> companies;
        std::istringstream compStream(companiesStr);
        std::string company;
        while (std::getline(compStream, company, ',')) {
            if (!company.empty()) companies.push_back(company);
        }

        GiftCard card(id, title, companies, balance, expiryDate);
        cards.push_back(card);
    }

    return cards;
}
