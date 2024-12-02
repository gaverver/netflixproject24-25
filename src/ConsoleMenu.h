#ifndef CONSOLEMENU_H
#define CONSOLEMENU_H
#include "IMenu.h"

#include <string>
#include <vector>
// ConsoleMenu class
class ConsoleMenu : public IMenu {
public:
    // method to print a given input to the console.
    void print(std::string output) override;
    // method to get an input from the user from the console.
    std::string scan() override;
    // method to get the command that the user enters.
    std::vector<std::string> nextCommand() override;
};
#endif