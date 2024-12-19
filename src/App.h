#ifndef APP_H
#define APP_H

#include "ICommand.h"
#include "IMenu.h"
#include "IDataBase.h"
#include "Runnable.h"
#include <vector>
#include <string>
#include <map>
#include <shared_mutex>

class App : public Runnable{
public:
    // constructor for app class that intiallizes the fields of App
    App(std::map<std::string, ICommand*> commands, IMenu& menu, std::shared_mutex& rw_mutex);
    // method to run the program using the user's input, implementing Runnable interface
    void run() override;

private:
    // menu that helps to get the command from the user
    IMenu& menu;
    // map that fits every command to it's command object
    std::map<std::string, ICommand*> commands;
    // mutex to lock the database when using him for a command
    // this is a mutex for the readers writers problem, that can either lock for writer or for reader
    std::shared_mutex& rw_mutex;
};

#endif
