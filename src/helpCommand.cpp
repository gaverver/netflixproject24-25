#include "helpCommand.h"

// constructor to set the values of the fields of the class
helpCommand::helpCommand(std::vector<ICommand*>& commands, IMenu* menu) : commands(commands), menu(menu) {
}

std::string helpCommand::description() const {
    // returns the description of the command(it is "help")
    return "help";
}


void helpCommand::execute(const std::vector<std::string>& args) {
    // iterates over all the commands inside the vector of commands and prints their description
    for (ICommand* command : commands) { 
        // checks if it is not null(so there will not be errors)
        if (command) { 
            menu->print(command->description());
        }
    }
}


