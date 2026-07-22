#ifndef VALIDATIONUTILS_H
#define VALIDATIONUTILS_H

#include <string>
#include <vector>

class ValidationUtils {
public:
    // Validates a date string is in strict DD-MM-YYYY format and is a real calendar date
    static bool isValidExpiryDate(const std::string& date);

    // The flat-file persistence format (FileRepository) uses '|' to separate
    // fields and ',' to separate companies within a field, with no escaping.
    // Reject these characters in user-entered title/company values so they
    // can never corrupt the on-disk format.
    static bool hasReservedDelimiter(const std::string& value);

    // Returns true if title or any company contains a reserved delimiter
    static bool hasInvalidDelimiters(const std::string& title, const std::vector<std::string>& companies);
};

#endif
