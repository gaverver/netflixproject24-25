#ifndef IMENU_H
#define IMENU_H

#include <string>
#include <vector>
class IMenu {
    public:
        // pure virtual function to print a given string
        virtual void print(std::string s) = 0;
        // pure virtual function to scan input from user
        virtual std::string scan() = 0;
        // pure virtual function to process the command from user
        virtual std::vector<std::string> nextCommand() = 0;
        // Virtual destructor to ensure proper cleanup in derived classes
        virtual ~IMenu() = default;
};

#endif
