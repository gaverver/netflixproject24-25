#include "helpCommand.h"

// constructor to set the values of the fields of the class
helpCommand::helpCommand(std::vector<ICommand*>& commands, IMenu& menu) : commands(commands), menu(menu) {
}

std::string helpCommand::description() const {
    // returns the description of the command(it is "help")
    return "help";
}


void helpCommand::execute(const std::vector<std::string>& args) {
    // invalid usage of help command - don't exec it and print an appropriate message
    if (args.size() != 0) {
        menu.print("400 Bad Request");
        return;
    }
    // the command is valid - print to the user in the beggining an appropriate message
    std::string commandsDesc = "200 Ok\n\n";
    // iterates over all the commands inside the vector of commands and prints their description
    for (ICommand* command : commands) { 
        // checks if it is not null(so there will not be errors)
        if (command) { 
            commandsDesc = commandsDesc + command->description() + '\n';
        }
    }
    // pop the '\n' in the end as the print function adds it.
    commandsDesc.pop_back();
    // prints the message and the commands' description
    menu.print(commandsDesc);
}

std::string helpCommand::rw_status() const {
    // the function returns "None" as help command isn't a reader or writer 
    return "None";
}


