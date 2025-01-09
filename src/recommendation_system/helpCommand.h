#ifndef HELPCOMMAND_H
#define HELPCOMMAND_H

#include "ICommand.h"
#include "IMenu.h"
#include <vector>
#include <string>

class helpCommand : public ICommand {
public:
    //  contructor of help command function
    helpCommand(std::vector<ICommand*>& commands, IMenu& menu);
    // function to execute the help command - needs to print all commands description.
    void execute(const std::vector<std::string>& args) override;
    // returns the description of the function
    std::string description() const override;
    std::string rw_status() const override;

private:
    IMenu& menu; // Reference to the menu - used for printing
    std::vector<ICommand*>& commands; // Reference to the commands vector - includes all the commands in the program
};

#endif
