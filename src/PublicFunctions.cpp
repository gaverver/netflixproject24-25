#include <string>
#include <cctype>
#include <iostream>
#include "PublicFunctions.h"
#include "IMenu.h"
#include "ConsoleMenu.h"
#include <fstream>
#include <sstream>
#include <vector>

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

// helper function to capture output from cout
std::string captureCommandOutput(ICommand* Runner, const std::vector<std::string>& args) {
    ConsoleMenu console;
    std::ostringstream outputBuffer;
    // Get the current buffer of cout and save it so it will be possible to restore later 
    std::streambuf* originalBuffer = std::cout.rdbuf();
    // Redirect cout to a stringstream so we could get to the output easily
    std::cout.rdbuf(outputBuffer.rdbuf()); 
    // Call the function execute - it prints to the screen the commands that the user can use. 
    Runner -> execute(args); 
    // Restore the original buffer, so that the next outputs will go there
    std::cout.rdbuf(originalBuffer);
    // Return the output of the function
    return outputBuffer.str();
}


//helper functio to remove the zeros at the beginning
std::string removeZeros(const std::string& num) {
    size_t firstNonZero = num.find_first_not_of('0');
    if (firstNonZero == std::string::npos) {
        //the number is all 0
        return "0";
    }
    return num.substr(firstNonZero);
}
//helper function to compare strings which represents two huge numbers
bool compareHugeNumbers(const std::string& num1, const std::string& num2) {
    //remove zeros
    std::string n1 = removeZeros(num1);
    std::string n2 = removeZeros(num2);
    //compare lengths
    if (num1.length() != num2.length()) {
        return num1.length() < num2.length();
    }
    return num1 < num2;
}