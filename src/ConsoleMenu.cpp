#include "ConsoleMenu.h"
#include <iostream>

void ConsoleMenu::print(std::string output) {
    // print the output to the cout with a new line. 
    std::cout << output << std::endl;
}

std::string ConsoleMenu::scan() {
    std::string input = "";
    // scna the input from the user(from cin) until the new line
    std::getline(std::cin, input);
    // return the scanned input
    return input;
}