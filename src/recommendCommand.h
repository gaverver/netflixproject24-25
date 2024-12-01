#ifndef RECOMMENDCOMMAND_H
#define RECOMMENDCOMMAND_H

#include <string>
#include <vector>
#include "ICommand.h"
#include "IDataBase.h"
#include "IMenu.h"

class recommendCommand : public ICommand {
private:
    // Reference to the database
    IdataBase& db;
    // Reference to the menu
    IMenu& menu;
    // helper function for casting from string to int
    int castStringToInt(std::string str, bool& isValid);

public:
    // Constructor
    recommendCommand(IdataBase& db2, IMenu& menu2);

    // Methods
    void execute(const std::vector<std::string>& args) override;
    std::string description() const override;
};

#endif