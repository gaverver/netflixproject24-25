#ifndef RECOMMENDCOMMAND_H
#define RECOMMENDCOMMAND_H

#include <string>
#include <vector>
#include "../../ICommand.h"
#include "../../IDataBase.h"
#include "../../IMenu.h"

class recommendCommand : public ICommand {
private:
    // Reference to the database
    IdataBase& db;
    // Reference to the menu
    IMenu& menu;
    // helper function for casting from string to int
    int castStringToInt(std::string str, bool& isValid);
    // helper function for swapping between two variables
    void swap(int& x, int& y);
    // this function will sort by rate and return the highest 10 (by selection algorithm)
    std::vector<int> sortUntilTen(std::map<int, int> rate);

public:
    // Constructor
    recommendCommand(IdataBase& db2, IMenu& menu2);

    // Methods
    void execute(const std::vector<std::string>& args) override;
    std::string description() const override;
};

#endif