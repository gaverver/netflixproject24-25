#include "AppTester.h"
#include <iostream>
#include <shared_mutex>

// implementing the constructor
AppTester::AppTester(std::map<std::string, ICommand*> commands, IMenu& menu, std::shared_mutex rw_mutex) : commands(commands), menu(menu), rw_mutex(rw_mutex) {}

void AppTester::run(int numCommands) {
    // iterate numCommands times - because it is the number of commands that the user entered(in the tests) and we need to scan and execute them 
    for (int i = 0; i < numCommands; i++) {
        // check if the user is still connected, if not stop running
        if (!menu.isConnected()) {
            break;
        }
        // get the next command from the user
        std::vector<std::string> args = menu.nextCommand();
        // seperate the command as the first argument is the command name and the others are the arguments for the function 
        std::vector<std::string> realArgs(args.begin() + 1, args.end());
        std::string rw_type = "";
        // try is for checking that the command is in the map of commands (a real command)
        try {
            // try to get the rw_status of the command that was entered by the user
            rw_type = commands[args[0]]->rw_status();
        } catch(...) {
            // if we reached here then there is no such command, and we prints the error message
            menu.print("400 Bad Request")
            // continue to the next command from user
            continue;
        }
        // the try worked and the command is real
        // check the type of the command, writer, reader or none
        switch (rw_type)
        {
        // if the command is a reader, shared lock is used to let others readers to read at the same time
        case "reader":
            std::shared_lock lock(rw_mutex);
            commands[arg[0]]->execute(realArgs);
            break;
        // if the command is a writer, unique lock is used to not let others do their critical section
        case "writer":
            std::unique_lock lock(rw_mutex);
            commands[arg[0]]->execute(realArgs);
            break;
        // if the command is neither a reader nor a writer, don't lock at all
        case "none":
            commands[arg[0]]->execute(realArgs);
            break;

        // not supposed to reach here
        default:
            break;
        }
    }
}