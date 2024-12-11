#include <string>
#include <cctype>
#include "PublicFunctions.h"

unsigned long int fromStringToULI(const std::string& str, bool& isValid) {
    isValid = true;

    // Check for empty string
    if (str.empty()) {
        isValid = false;
        return 0;
    }

    // ensure all characters are digits
    for (char ch : str) {
        if (!std::isdigit(ch)) {
            isValid = false;
            return 0;
        }
    }

    // another check to know if num is out_of_range(above the maximum unsigned long int)
    try {
        // casting...
        unsigned long int num2 = std::stoul(str);
    // catching out_of_range exception
    } catch (...) {
        isValid = false;
        return 0;
    }

    // convert the string to an unsigned long int
    // use a larger type temporarily to know if we are out of range
    unsigned long long num = 0;
    for (char ch : str) {
        // get the number from the string by an easy algorithm
        num = num * 10 + (ch - '0');
    }

    // returns the number in unsigned long int
    return static_cast<unsigned long int>(num);
}