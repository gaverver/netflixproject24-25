#ifndef PATCHCOMMAND_H
#define PATCHCOMMAND_H

#include "ICommand.h"
#include "IDataBase.h"
#include "IMenu.h"
#include <vector>
#include <string>

class PATCHCommand : public ICommand {
public:
    //  contructor of help command function
    PATCHCommand(IDataBase& UMManager, IMenu& menu);
    // function to execute the help command - needs to print all commands description.
    void execute(const std::vector<std::string>& args) override;
    // returns the description of the function
    std::string description() const override;
    // returns if the command is reader/writer/none
    std::string rw_status() const override;

private:
    // the database that the users and movies details are stored in
    IDataBase& UMManager;
    // menu that writes the output of the command
    IMenu& menu;
};

#endif