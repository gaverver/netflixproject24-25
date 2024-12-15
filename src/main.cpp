#include "IMenu.h"
#include "ICommand.h"
#include "addCommand.h"
#include "helpCommand.h"
#include "GETCommand.h"
#include "App.h"
#include "ConsoleMenu.h"
#include "DBFile.h"
#include "IDataBase.h"

int main(int argc, char** argv) {
    // construct the app to be able to run it.
    std::map<std::string, ICommand*> commands;
    std::vector<ICommand*> helpCommands;
    // set up the databse that the app data will be stored in
    DBFile dbfile("../data");
    IDataBase& data = dbfile;
    ConsoleMenu cmenu;
    IMenu& menu = cmenu;
    // set up the command instances that will execute the commands
    ICommand* add = new addCommand(data);
    ICommand* GET = new GETCommand(data, menu);
    ICommand* help = new helpCommand(helpCommands, menu);
    // add the command to the vector of help commands that help command will use
    helpCommands.push_back(add);
    helpCommands.push_back(GET);
    helpCommands.push_back(help);
    // map the commands to the appropriate instances of commands
    commands["add"] = add;
    commands["help"] = help;
    commands["GET"] = GET;
    // create App object
    App app(commands, menu);
    // Call the run function - this will run the app
    app.run();
}