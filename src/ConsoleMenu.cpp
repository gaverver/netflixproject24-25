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

std::vector<std::string> ConsoleMenu::nextCommand() {
    // Get the command that the user entered.
    std::string input = scan();
    std::vector<std::string> commands;
    size_t pos = 0;
    // find the position of the first space in the input to know when the word ends. It repeats this until the string is empty(nothing to read).
    while ((pos = input.find(' ')) != std::string::npos) {
        // Get the first word in the string, it is achived be reading everything until the space char.  
        std::string command = input.substr(0, pos);
        if (command != "") {
            commands.push_back(command);
        }
        // erase the readed word and the space to avoid reading the space and the same word again. 
        input.erase(0, pos + 1);
    }
    // if the input is still not empty and doesn't equal the empty string, we missed something and we need to add it to the vector.
    if (!input.empty() && input != "") {
        commands.push_back(input);
    }
    // if nothing was entered by user, the function should return vector in size one with the string ""
    if (commands.size() == 0) {
        commands.push_back("");
    }
    return commands;
}
