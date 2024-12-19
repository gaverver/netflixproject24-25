#include "App.h"
#include <iostream>
#include <mutex>
#include <map>
#include <vector>
#include <string>

// constructor
App::App(std::map<std::string, ICommand*> commands, IMenu& menu, std::shared_mutex& rw_mutex)
    : commands(commands), menu(menu), rw_mutex(rw_mutex) {}

/// destructor
App::~App() {
    for (auto& [key, command] : commands) {
        delete command;
    }
    commands.clear();
}

// run function
void App::run() {
    // run indefinitely - the user can enter input anytime he wants since he starts running the program
    while (true) {
        // if the user isn't connected, stop running
        if (!menu.isConnected()) {
            break;
        }

        // getting the next command the user send
        std::vector<std::string> args = menu.nextCommand();
        // if he sended nothing, it's invalid
        if (args.empty()) {
            menu.print("400 Bad Request");
            continue;
        }

        // spliting the args to the command and the arguments to the command
        std::vector<std::string> realArgs(args.begin() + 1, args.end());
        std::string rw_type;

        if (commands.find(args[0]) != commands.end()) {
            try {
            // try to get the type of the command (reader, writer or none)
            rw_type = commands[(args[0])]->rw_status();
            } catch (...) {
                // reached here if the command isn't a real command
                menu.print("400 Bad Request");
                continue;
            }
            
            // check the type of the command, and lock appropriately
            // only this three options are available
            if (rw_type == "reader") {
                std::shared_lock lock(rw_mutex);
                commands[args[0]]->execute(realArgs);
            } else if (rw_type == "writer") {
                std::unique_lock lock(rw_mutex);
                commands[args[0]]->execute(realArgs);
            } else if (rw_type == "none") {
                commands[args[0]]->execute(realArgs);
            }
        } else {
            menu.print("400 Bad Request");
            continue;;
        }
    }
}
