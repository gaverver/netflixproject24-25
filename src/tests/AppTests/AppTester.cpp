#include "AppTester.h"
#include <iostream>
AppTester::AppTester(std::map<std::string, ICommand*> commands, IMenu& menu) : commands(commands), menu(menu) {}

void AppTester::run(int numCommands) {
    // iterate numCommands times - because it is the number of commands that the user entered(in the tests) and we need to scan and execute them 
    for (int i = 0; i < numCommands; i++) {
        // get the next command from the user
        std::vector<std::string> args = menu.nextCommand();
        // seperate the command as the first argument is the command name and the others are the arguments for the function 
        std::vector<std::string> realArgs(args.begin() + 1, args.end());
        // check that the command is in the map of commands. 
        if (commands.find(args[0]) != commands.end()) {
            try {
                // try to execute the command that was entered by the user - if it fails(exception) then do nothing
                commands[args[0]]->execute(realArgs);
            } catch(...) {

            }
        }
    }
}