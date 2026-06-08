#include <vector>
#include <string>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "../include/GiftCard.h"


GiftCard :: GiftCard(int id, const std::string& title, const std::vector<std::string>& companies, double balance, const std::string& expiryDate):
id(id), title(title), companies(companies), balance(balance), expiryDate(expiryDate) {
    updateDaysLeft();
}




int GiftCard :: getId() const {
    return id;
}
std::string GiftCard :: getTitle() const {
    return title;
}
std::vector<std::string> GiftCard :: getCompanies() const {
    return companies;
}
double GiftCard :: getBalance() const {
    return balance;
}
std::string GiftCard::getExpiryDate() const {
    return expiryDate;
}
int GiftCard :: getDaysToExpiry() const {
    return daysLeft;
}


void GiftCard :: setTitle(const std::string& newTitle) {
    title = newTitle;
}
void GiftCard :: setBalance(double newBalance) {
    balance = newBalance;
}

void GiftCard :: addBalance(double amount) {
    balance += amount;
}
void GiftCard :: deductBalance(double amount) {
    if (amount > balance) {
        throw std::runtime_error("Insufficient balance");
    }
    balance -= amount;
}
void GiftCard :: setExpiryDate(const std::string& newExpiryDate) {
    expiryDate = newExpiryDate;
    updateDaysLeft();
}
void GiftCard :: addCompany(const std::string& company) {
    companies.push_back(company);
}







bool GiftCard :: isExpired() {
    updateDaysLeft();
    return daysLeft <= 0;
}

bool GiftCard :: isExpiringSoon() {
    updateDaysLeft();
    if(daysLeft < 0) {
        return false; // Already expired
    }
    return daysLeft <= 30; // This is a placeholder. You would need to implement the actual logic for determining if the card is expiring soon.
}



std::string GiftCard::getTodayDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::ostringstream oss;

    oss << std::setw(2) << std::setfill('0') << localTime->tm_mday << "-"
        << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "-"
        << (localTime->tm_year + 1900);

    return oss.str();
}

void GiftCard :: updateDaysLeft() {
    std::string today = getTodayDate();

    std::tm expiryTm = {};
    std::istringstream ss(expiryDate);
    ss >> std::get_time(&expiryTm, "%d-%m-%Y");
    if (ss.fail()) {
        throw std::runtime_error("Invalid expiry date format");
    }

    std::tm todayTm = {};
    ss.clear();
    ss.str(today);
    ss >> std::get_time(&todayTm, "%d-%m-%Y");
    if (ss.fail()) {
        throw std::runtime_error("Invalid today date format");
    }

    std::time_t expiryTime = std::mktime(&expiryTm);
    std::time_t todayTime = std::mktime(&todayTm);

    double secondsLeft = difftime(expiryTime, todayTime);
    daysLeft = static_cast<int>(secondsLeft / (60 * 60 * 24));
}



