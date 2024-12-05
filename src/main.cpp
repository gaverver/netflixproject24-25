#include "IMenu.h"
#include "ICommand.h"
#include "addCommand.h"
#include "helpCommand.h"
#include "recommendCommand.h"
#include "App.h"
#include "ConsoleMenu.h"
#include "DBFile.h"

int main(int argc, char** argv) {
    // construct the app to be able to run it.
    std::map<std::string, ICommand*> commands;
    std::vector<ICommand*> helpCommands;
    // set up the databse that the app data will be stored in
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    IMenu* helpMenu = new ConsoleMenu();
    // set up the command instances that will execute the commands
    ICommand* add = new addCommand(data);
    ICommand* recommend = new recommendCommand(data, menu);
    ICommand* help = new helpCommand(helpCommands, helpMenu);
    // add the command to the vector of help commands that help command will use
    helpCommands.push_back(add);
    helpCommands.push_back(recommend);
    helpCommands.push_back(help);
    // map the commands to the appropriate instances of commands
    commands["add"] = add;
    commands["help"] = help;
    commands["recommend"] = recommend;
    // create App object
    App app(commands, menu);
    // Call the run function - this will run the app
    app.run();
}