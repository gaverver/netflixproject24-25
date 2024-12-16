#ifndef POSTCOMMAND_H
#define POSTCOMMAND_H

#include "ICommand.h"
#include "IDataBase.h"
#include <vector>
#include <string>

class POSTCommand : public ICommand {
public:
    //  contructor of help command function
    POSTCommand(IDataBase& UMManager);
    // function to execute the help command - needs to print all commands description.
    void execute(const std::vector<std::string>& args) override;
    // returns the description of the function
    std::string description() const override;
    // returns if the command is reader/writer/none
    std::string rw_status() const override;

private:
    // the database that the users and movies details are stored in
    IDataBase& UMManager;
};

#endif