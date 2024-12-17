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

        // virtual function to process the command from user
        virtual std::vector<std::string> nextCommand() {
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
        // pure virtual function to check if the menu is connected to the source
        virtual bool isConnected() = 0;

        // Virtual destructor to ensure proper cleanup in derived classes
        virtual ~IMenu() = default;
};

#endif
