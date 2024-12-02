#ifndef RECOMMENDCOMMAND_H
#define RECOMMENDCOMMAND_H

#include <string>
#include <vector>
#include "ICommand.h"
#include "IDataBase.h"
#include "IMenu.h"

class recommendCommand : public ICommand {
private:
    // reference to the database, to use the data
    IdataBase& db;
    // reference to the menu, to print via IMenu in the execute function
    IMenu& menu;
    // helper function for casting from string to int
    int castStringToInt(std::string str, bool& isValid);

public:
    // constructor
    recommendCommand(IdataBase& db, IMenu& menu);

    // methods inherited from Command class
    void execute(const std::vector<std::string>& args) override;
    std::string description() const override;
};

#endif