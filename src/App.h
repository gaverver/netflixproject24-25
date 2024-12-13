#ifndef APP_H
#define APP_H

#include "ICommand.h"
#include "IMenu.h"
#include "IDataBase.h"
#include "Runnable.h"
#include <vector>
#include <string>
#include <map>

class App : public Runnable{
public:
    // constructor for app class that intiallizes the fields of App
    App(std::map<std::string, ICommand*> commands, IMenu& menu);
    // method to run the program using the user's input, implementing Runnable interface
    void run() override;

private:
    // menu that helps to get the command from the user
    IMenu& menu;
    // map that fits every command to it's command object
    std::map<std::string, ICommand*> commands;
};

#endif