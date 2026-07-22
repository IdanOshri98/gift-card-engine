#include "../include/ValidationUtils.h"
#include <sstream>
#include <iomanip>
#include <ctime>

bool ValidationUtils::isValidExpiryDate(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%d-%m-%Y");
    if (ss.fail() || !ss.eof()) {
        return false;
    }

    // std::get_time accepts calendar-impossible dates (e.g. 31-02-2026)
    // without failing, so check them by normalizing through mktime: it
    // rolls invalid day/month combinations forward into the next month,
    // which we detect by comparing the day before and after normalization.
    int originalDay = tm.tm_mday;
    std::tm normalized = tm;
    std::mktime(&normalized);
    return normalized.tm_mday == originalDay && normalized.tm_mon == tm.tm_mon;
}

bool ValidationUtils::hasReservedDelimiter(const std::string& value) {
    return value.find('|') != std::string::npos || value.find(',') != std::string::npos;
}

bool ValidationUtils::hasInvalidDelimiters(const std::string& title, const std::vector<std::string>& companies) {
    if (hasReservedDelimiter(title)) {
        return true;
    }
    for (const std::string& company : companies) {
        if (hasReservedDelimiter(company)) {
            return true;
        }
    }
    return false;
}
