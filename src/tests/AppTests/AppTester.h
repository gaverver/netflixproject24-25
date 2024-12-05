#ifndef APPTESTER_H
#define APPTESTER_H

#include "../../ICommand.h"
#include "../../IMenu.h"
#include "../../IDataBase.h"
#include <vector>
#include <string>
#include <map>

class AppTester {
public:
    // constructor for app class that intiallizes the fields of AppTester
    AppTester(std::map<std::string, ICommand*> commands, IMenu& menu);
    // method to run the program using the user's input(and number of commands that he enters)
    void run(int numCommands);

private:
    // menu that helps to get the command from the user
    IMenu& menu;
    // map that fits every command to it's command object
    std::map<std::string, ICommand*> commands;
};

#endif