#ifndef GIFT_CARD_H
#define GIFT_CARD_H

#include <string>
#include <vector>

class GiftCard {
    private:
        int id;
        std::string title;
        std::vector<std::string> companies;
        double balance;
        std::string expiryDate;
        

    public:

        GiftCard(int id, const std::string& title, const std::vector<std::string>& companies, double balance, const std::string& expiryDate);

        std::vector<std::string> getCompanies() const;
        std::string getTitle() const;
        double getBalance() const;
        std::string getExpiryDate() const;
        int getId() const;
        int getDaysToExpiry() const;
        std::string getTodayDate() ;

        void addBalance(double amount);
        void deductBalance(double amount);
        void setTitle(const std::string& newTitle);
        void setBalance(double newBalance);
        void setExpiryDate(const std::string& newExpiryDate);
        void updateDaysLeft();
        void addCompany(const std::string& company);

        bool isExpired();
        bool isExpiringSoon() ;
         

    protected:
        int daysLeft;
};

#endif 